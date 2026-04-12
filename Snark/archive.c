/**
* This is the archive.c program. It makes, saves, adds, loads, removes, and extracts archives
* @file archive.c
* @author Jacob McLain
*/

#include "archive.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "compress.h"
#include "error.h"
#include "io.h"

/** Representation for a node on a linked list of FileRec objects. */
struct FileNodeStruct {
  /** FileRec struct stored inside this node (rather than by pointer). */
  FileRec rec;
  
  /** Pointer to the next node on the linked list. */
  struct FileNodeStruct *next;
};

/** Shorter name for FileNodeStruct */
typedef struct FileNodeStruct FileNode;

/** Representation of an archive, a linked list of FileNodes. */
struct ArchiveStruct {
  /** Head pointer of the head of a list of FileNodes. */
  FileNode *head;
};


/**
* This function dynamically allocates
* a new, empty archive.
* @return arc an archive
*/
Archive *makeArchive() 
{
    Archive *arc = malloc(sizeof(Archive));
    if(!arc) {
        return NULL;
    }
    arc->head = NULL;
    return arc;
}

/**
* This function frees all the memory used to store an
* archive, including the Archive struct, all the nodes
* on the linked list and all the dynamically allocated
* memory used in each node.
* @param arc the archive
*/
void freeArchive( Archive *arc )
{
    if(!arc) {
        return;
    }
    FileNode *cur = arc->head;
    while(cur) {
        FileNode *next = cur->next;
        FileRec *rec = &cur->rec;

        free(rec->name);

        if(rec->raw) {
            free(rec->raw->data);
            free(rec->raw);
        }
        if(rec->comp) {
            free(rec->comp->data);
            free(rec->comp);
        }

        free(cur);
        cur = next;
    }

    free(arc);
    
}

/**
* This function reads the contents of an archive
* from the file with the given name, returning a new, dynamically allocated Archive.
* @param fname the filename
* @return arc the archive
*/
Archive *loadArchive(char const *fname) {

    FILE *fp = fopen(fname, "rb");

    if(!fp) {
        setErrorMessage("Can't open archive");
        return NULL;
    }

    Archive *arc = makeArchive();

    if(!arc) {
        fclose(fp);
        setErrorMessage("Out of memory");
        return NULL;
    }

    while(true) {

        char name[1024];
        int idx = 0;
        int c;

        while((c = fgetc(fp)) != EOF && c != 0 && idx < sizeof(name)-1) {
            name[idx++] = (char)c;
        }

        if(c == EOF) {
            if(idx != 0) { 
                freeArchive(arc);
                setErrorMessage("Invalid archive");
                fclose(fp);
                return NULL;
            }
            break;
        }

        name[idx] = '\0';

        unsigned char b[4];

        if(fread(b, 1, 4, fp) != 4) { 
            freeArchive(arc);
            setErrorMessage("Invalid archive");
            fclose(fp);
            return NULL;
        }

        unsigned int compLen = b[0] | (b[1]<<8) | (b[2]<<16) | (b[3]<<24);

        Buffer *comp = NULL;

        if(compLen > 0) {

            comp = malloc(sizeof(Buffer));

            if(!comp) {
                freeArchive(arc);
                setErrorMessage("Out of memory");
                fclose(fp);
                return NULL;
            }

            comp->data = malloc(compLen);

            if(!comp->data) {
                free(comp);
                freeArchive(arc);
                setErrorMessage("Out of memory");
                fclose(fp);
                return NULL;
            }

            if(fread(comp->data, 1, compLen, fp) != compLen) {
                free(comp->data); free(comp); freeArchive(arc);
                setErrorMessage("Invalid archive");
                fclose(fp);
                return NULL;
            }

            comp->len = compLen;
            comp->cap = compLen;
            comp->pos = 0;

        }

        FileNode *node = malloc(sizeof(FileNode));
        if(!node) {
            if(comp) {
                free(comp->data);
                free(comp);
            }
            freeArchive(arc);
            setErrorMessage("Out of memory");
            fclose(fp);
            return NULL;
        }

        node->rec.name = strdup(name);
        node->rec.raw = NULL;
        node->rec.comp = comp;
        node->next = arc->head;
        arc->head = node;
    }

    fclose(fp);
    return arc;

}




/**
* This function adds a file with the given name to the 
* archive. It returns false if it’s not successful 
* (e.g., if the given file can’t be opened or if a 
* file with the given name is already part of the archive).
* On an error, this function should use the error component
* to set an appropriate error message that can be reported by client code.
* @param arc the archive
* @param fname the filename
* @return T or F based on if the archive was added successfully
*/
bool addArchive(Archive *arc, char const *fname) {

    if(!arc) {
        setErrorMessage("Archive is empty");
        return false;
    }

    if(!fname || fname[0] == '\0') {
        setErrorMessage("No filename provided");
        return false;
    }

    if(strchr(fname, '/')) {
        setErrorMessage("Invalid file name");
        return false;
    }

    for(FileNode *n = arc->head; n; n = n->next) {
        if(strcmp(n->rec.name, fname) == 0) {
            setErrorMessage("Archive already contains file");
            return false;
        }
    }

    Buffer *raw = readFile(fname);

    if(!raw) {
        setErrorMessage("Can't read file");
        return false;
    }

    Buffer *comp = compressData(raw);

    if(!comp) {
        freeBuffer(raw);
        setErrorMessage("Compression failed");
        return false;
    }

    FileNode *node = malloc(sizeof(FileNode));
    if(!node) {
        freeBuffer(comp);
        freeBuffer(raw);
        setErrorMessage("Out of memory");
        return false;
    }

    node->rec.name = strdup(fname);
    node->rec.raw = raw;
    node->rec.comp = comp;
    node->next = arc->head;
    arc->head = node;
    return true;

}

/**
* This function removes a file with the given name
* from the given archive. It should unlink the node
* for the file from the linked list of FileNodes and
* should free any memory used to represent the file.
* On an error, this function should use the error
* component to set an appropriate error message and return false.
* @param arc the archive
* @param fname the name of the file
* @return T or F based on if the archive was removed successfully
*/
bool removeArchive( Archive *arc, char const *fname )
{
    if(!arc) {
        setErrorMessage("Archive is empty");
        return false;
    }

    FileNode *cur = arc->head;
    FileNode *prev = NULL;

    while(cur && strcmp(cur->rec.name, fname) != 0) {
        prev = cur;
        cur = cur->next;
    }

    if(!cur) {
        setErrorMessage("Archive doesn't contain file");
        return false;
    }

    if(prev) {
        prev->next = cur->next;
    }

    else {
        arc->head = cur->next;
    }

    free(cur->rec.name);

    if(cur->rec.raw) {
        free(cur->rec.raw->data);
        free(cur->rec.raw);
    }

    if(cur->rec.comp) {
        free(cur->rec.comp->data);
        free(cur->rec.comp);
    }

    free(cur);
    return true;

}

/**
* This function extracts a file with the given name
* from the given archive, creating a copy of that 
* file in the current directory. This function doesn’t
* modify the archive; it just creates a copy of one of
* the files in the archive. On an error, it should use
* the error component to set an appropriate error message
* and return false.
* @param arc the archive
* @param fname the name of the file
* @return T or F based on if the archive was extracted successfully
*/
bool extractArchive(Archive *arc, const char *fname)
{
    if(!arc) {
        setErrorMessage("Archive is empty");
        return false;
    }

    FileNode *n = arc->head;
    while(n && strcmp(n->rec.name, fname) != 0)
        n = n->next;

    if(!n) {
        setErrorMessage("File not found in archive");
        return false;
    }
    Buffer *out = n->rec.raw;
    if(!out && n->rec.comp) {
        out = uncompressData(n->rec.comp);
        if(!out) {
            setErrorMessage("Failed to uncompress file");
            return false;
        }
        n->rec.raw = out;
    }
    if(!writeFile(fname, out)) {
        setErrorMessage("Error writing extracted file");
        return false;
    }
    return true;
}



/**
* This function saves a copy of the archive to a file
* with the given name, using the format described above
* under “Saved Archive Representation”. This function 
* doesn’t change the archive, it just saves a copy. 
* On an error, it should use the error component to
* set an appropriate error message and return false.
* @param arc the archive
* @param fname the name of the file
* @return T or F based on if the archive was saved successfully
*/
bool saveArchive(Archive *arc, const char *fname) {
    if(!arc || !fname) {
       return false; 
    }

    FILE *fp = fopen(fname, "wb");
    if(!fp) {
        return false;
    }

    int nFiles = 0;
    for(FileNode *n = arc->head; n; n = n->next) {
        nFiles++;
    }

    if(nFiles == 0) {
        fclose(fp);
        return true;
    }

    FileRec **files = malloc(sizeof(FileRec*) * nFiles);
    if(!files) {
        fclose(fp);
        return false;
    }

    int i = 0;
    for(FileNode *n = arc->head; n; n = n->next) {
        files[i++] = &n->rec;
    }

    int cmpFileRec(const void *a, const void *b) {
        FileRec *fa = *(FileRec**)a;
        FileRec *fb = *(FileRec**)b;
        return strcmp(fa->name, fb->name);
    }

    qsort(files, nFiles, sizeof(FileRec*), cmpFileRec);

    for(i = 0; i < nFiles; i++) {
        FileRec *rec = files[i];
        int nameLen = strlen(rec->name) + 1;
        if(fwrite(rec->name, 1, nameLen, fp) != nameLen) {
            free(files);
            fclose(fp);
            return false;
        }

        unsigned int compLen = rec->comp ? rec->comp->len : 0;
        unsigned char b[4] = {(compLen & 0xFF), ((compLen >> 8) & 0xFF), ((compLen >> 16) & 0xFF), ((compLen >> 24) & 0xFF)};

        if(fwrite(b, 1, 4, fp) != 4) {
            free(files);
            fclose(fp);
            return false;
        }

        if(compLen > 0 && fwrite(rec->comp->data, 1, compLen, fp) != compLen) {
            free(files);
            fclose(fp);
            return false;
        }

    }

    free(files);
    fclose(fp);
    return true;

}
/**
* This function lets client code see the contents
* of an archive without being able to directly 
* access the representation of the archive.
* It visits every file in the archive, in order
* and calls the visitor function, passing it a 
* pointer to the FileRec part of each node. The
* context pointer lets client code pass additional
* information through the function. The traverseArchive()
* function just needs to pass the given context pointer as
* the second parameter every time it calls the visitor function.
* Client code can use this to pass additional information that’s
* needed during traversal.
* @param arc the archive
* @param visitor a helper method to find the visitor
* @param context context to help find the visitor
*/
void traverseArchive( Archive *arc, void visitor( FileRec *rec, void *context ), void *context )
{
    if(!arc || !visitor) {
		return;
	}
    
    for(FileNode *n = arc->head; n; n = n->next) {
        visitor(&n->rec, context);
    }
}
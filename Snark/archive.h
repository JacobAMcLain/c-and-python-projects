/**
* This is the archive.c program. It makes, saves, adds, loads, removes, and extracts archives
* @file archive.h
* @author Jacob McLain
*/

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "io.h"
#include "compress.h"
#include "error.h"

typedef struct {
  /** Name and path of the file. */
  char *name;

  /** Raw, uncompressed copy of the file's contents. */
  Buffer *raw;

  /** Comrpessed copy of the file's contents. */
  Buffer *comp;
} FileRec;

/** Incomplete type for the archive. */
typedef struct ArchiveStruct Archive;


/**
* This function dynamically allocates
* a new, empty archive.
*/
Archive *makeArchive();

/**
* This function frees all the memory used to store an
* archive, including the Archive struct, all the nodes
* on the linked list and all the dynamically allocated
* memory used in each node.
*/
void freeArchive( Archive *arc );

/**
* This function reads the contents of an archive
* from the file with the given name, returning a new, dynamically allocated Archive.
*/
Archive *loadArchive( char const *fname );

/**
* This function adds a file with the given name to the 
* archive. It returns false if it’s not successful 
* (e.g., if the given file can’t be opened or if a 
* file with the given name is already part of the archive).
* On an error, this function should use the error component
* to set an appropriate error message that can be reported by client code.
*/
bool addArchive( Archive *arc, char const *fname );

/**
* This function removes a file with the given name
* from the given archive. It should unlink the node
* for the file from the linked list of FileNodes and
* should free any memory used to represent the file.
* On an error, this function should use the error
* component to set an appropriate error message and return false.
*/
bool removeArchive( Archive *arc, char const *fname );

/**
* This function extracts a file with the given name
* from the given archive, creating a copy of that 
* file in the current directory. This function doesn’t
* modify the archive; it just creates a copy of one of
* the files in the archive. On an error, it should use
* the error component to set an appropriate error message
* and return false.
*/
bool extractArchive( Archive *arc, char const *fname );

/**
* This function saves a copy of the archive to a file
* with the given name, using the format described above
* under “Saved Archive Representation”. This function 
* doesn’t change the archive, it just saves a copy. 
* On an error, it should use the error component to
* set an appropriate error message and return false.
*/
bool saveArchive( Archive *arc, char const *fname );

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
*/
void traverseArchive( Archive *arc, void visitor( FileRec *rec, void *context ), void *context );

#endif

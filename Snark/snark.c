/**
* This is the main class. It gets user input and uses handleCommand to handle what the user tells it
* @file snark.c
* @author Jacob McLain
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <setjmp.h>
#include "archive.h"
#include "error.h"
#include "io.h"
#include "compress.h"

/** The minimum name length */
#define MIN_NAME_LEN 20

/** 
* a helper for comparing FileRec for qsort 
* @param a pointer to a void variable
* @param b pointer to a void variable
* @return an int based on if a or b came first
*
*/
int cmpFileRec(const void *a, const void *b) {
    FileRec *fa = *(FileRec**)a;
    FileRec *fb = *(FileRec**)b;

    return strcmp(fa->name, fb->name);
}

/** 
* Counts the number of files 
* @param rec the FileRec
* @param ctx the context
*/
void countFiles(FileRec *rec, void *ctx) {
    (void)rec;
    int *count = ctx;
    (*count)++;
}
/**
* Collects the files context
* @param rec the FileRec
* @param ctx the context
*/
void collectFilesCtx(FileRec *rec, void *ctx) {
    struct {
        FileRec **arr;
        int idx;
    } *c = ctx;
    c->arr[c->idx++] = rec;
}
/**
* This function handles various commands and utilizes jumps and the error class to figure out what to do with them
* @param arc the archive
* @param line the line
* @param env the jump buffer environment
* @return T or F based on if the command was handled
*/
bool handleCommand(Archive *arc, char *line, jmp_buf *env) {

    while(*line && isspace((unsigned char)*line)) {
        line++;
    }

    if(*line == '\0' || *line == '#') {
        return false;
    }

    char command[256];

    if(sscanf(line, "%255s", command) != 1) {
        setErrorMessage("Invalid command");
        longjmp(*env, 1);
    }

    for(char *p = command; *p; p++) {
        *p = (char)tolower((unsigned char)*p);
    }

    line += strlen(command);

    while(*line && isspace((unsigned char)*line)) line++;
    char fname[1024];
    if(strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
        return true;
    }

    else if(strcmp(command, "add") == 0) {
        if(sscanf(line, "%1023s", fname) != 1) {
            setErrorMessage("No filename provided for add");
            longjmp(*env, 1);
        }
        if(!addArchive(arc, fname)) {
            longjmp(*env, 1);
        }
    }

    else if(strcmp(command, "extract") == 0) {
        if(sscanf(line, "%1023s", fname) != 1) {
            setErrorMessage("No filename provided for extract");
            longjmp(*env, 1);
        }
        if(!extractArchive(arc, fname)) longjmp(*env, 1);
    }

    else if(strcmp(command, "remove") == 0) {
        if(sscanf(line, "%1023s", fname) != 1) {
            setErrorMessage("No filename provided for remove");
            longjmp(*env, 1);
        }
        if(!removeArchive(arc, fname)) longjmp(*env, 1);
    }

    else if(strcmp(command, "save") == 0) {
        if(sscanf(line, "%1023s", fname) != 1) {
            setErrorMessage("No filename provided for save");
            longjmp(*env, 1);
        }
        if(!saveArchive(arc, fname)) longjmp(*env, 1);
    }

    else if(strcmp(command, "show") == 0) {
        int nFiles = 0;
        
        traverseArchive(arc, countFiles, &nFiles);

        if(nFiles == 0) {
            printf("Archive is empty\n");
        } 

        else {

            FileRec **files = malloc(sizeof(FileRec*) * nFiles);

            if(!files) {
                setErrorMessage("Out of memory");
                longjmp(*env, 1);
            }

            struct { 
                FileRec **arr;
                int idx;
            } collectCtx = { files, 0 };
            
            traverseArchive(arc, collectFilesCtx, &collectCtx);
            qsort(files, nFiles, sizeof(FileRec*), cmpFileRec);
            int maxNameLen = MIN_NAME_LEN;

            for(int i = 0; i < nFiles; i++) {
                int len = strlen(files[i]->name);
                if(len > maxNameLen) {
                    maxNameLen = len; 
                } 
            }

            printf("%-*s%8s%9s\n", maxNameLen+1, "File", "orig", "comp");
            for(int i = 0; i < nFiles; i++) {
                FileRec *rec = files[i];
                if(!rec->raw && rec->comp) rec->raw = uncompressData(rec->comp);
                printf("%-*s%8d%9d\n", (int)maxNameLen+1, rec->name, rec->raw ? rec->raw->len : 0, rec->comp ? rec->comp->len : 0);
            }

            free(files);

        }
    }
    else {

        setErrorMessage("Unknown command");
        longjmp(*env, 1);

    }

    return false;
}

/** 
* This is the main method. It gets user input and uses handleCommand to handle what the user tells it 
* @param argc the amount of arguments
* @param argv the arguments in the command line in array form
* @return 0 if success, anything else if fail
*/
int main(int argc, char *argv[]) {

    Archive *arc = NULL;
    FILE *input = stdin;
    bool scriptMode = false;
    char *archiveFile = NULL;
    char *scriptFile = NULL;
    bool aSeen = false;

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-a") == 0) {

            if(aSeen) {
                fprintf(stderr, "usage: snark [-a archive-file] [-s script-file]\n");
                return EXIT_FAILURE;
            }

            aSeen = true;

            if(i++ >= argc || argv[i][0] == '-') {
                fprintf(stderr, "usage: snark [-a archive-file] [-s script-file]\n");
                return EXIT_FAILURE;
            }

            archiveFile = argv[i];

        } 

        else if(strcmp(argv[i], "-s") == 0) {
            if(i++ >= argc || argv[i][0] == '-') {
                fprintf(stderr, "usage: snark [-a archive-file] [-s script-file]\n");
                return EXIT_FAILURE;
            }

            scriptFile = argv[i];
            scriptMode = true;

        } 

        else {
            fprintf(stderr, "usage: snark [-a archive-file] [-s script-file]\n");
            return EXIT_FAILURE;
        }
    }
    if(archiveFile) {

        arc = loadArchive(archiveFile);
        if(!arc) {
            const char *msg = getErrorMessage();
            fprintf(stderr, "%s\n", msg ? msg : "Can't open archive");
            return EXIT_FAILURE;
        }
    }
    else {

        arc = makeArchive();
        if(!arc) {
            fprintf(stderr, "Out of memory creating archive\n");
            return EXIT_FAILURE;
        }

    }
    if(scriptMode) {

        input = fopen(scriptFile, "r");

        if(!input) {
            fprintf(stderr, "Error: cannot open script file '%s'\n", scriptFile);
            freeArchive(arc);
            return EXIT_FAILURE;
        }

    }
    char line[1024];

    while (true) {
        if(!scriptMode) {
            printf("cmd> ");
        }

        if(!fgets(line, sizeof(line), input)) {
            break;
        }

        int len = strlen(line);
        while(len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }

        if(!scriptMode) {
            printf("%s\n", line);
        }

        jmp_buf env;
        if(setjmp(env) != 0) {
            const char *msg = getErrorMessage();
            printf("%s\n", msg && msg[0] ? msg : "Invalid command");
            setErrorMessage(NULL);
            continue;
        }

        if(handleCommand(arc, line, &env)) {
            break;
        }

    }
    if(input != stdin) {
        fclose(input);
    }
    
    freeArchive(arc);
    return EXIT_SUCCESS;
}

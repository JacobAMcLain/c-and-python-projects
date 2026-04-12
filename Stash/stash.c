/**
* This program simulates a basic command-line interpreter on an Unix system
* @file stash.c
* @author Jacob McLain
*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>     
#include <unistd.h>    
#include <stdbool.h> 
#include <sys/types.h>  
#include <sys/wait.h>
#include <ctype.h>

/** The max length of a line */
#define MAX_LINE 1024
/** The max number of words */
#define MAX_WORDS 513   

/**
* This function takes a user command (line) as input. It breaks the line into
* individual words, adds null termination between the words so each word is a separate string, and
* it fills in a pointer in the words array to point to the start of each word. It returns the number
* of words found in the given line. The words array should be at least 513 elements in length, so it
* has room for the largest possible number of words that could fit in a 1024-character input line.
* @param line the line that the command is being parsed from
* @param words the words that the command consists of
* @return count the amount of words on the line
*/
int parseCommand( char *line, char *words[] ) {

    int count = 0;
    bool inWord = false; // Check for being in word (0 if not in word)
    // Splits lines into words, stores pointers at the beginning of each word in "words", and null terminates the last pointer
    for ( int i = 0; line[i] != '\0'; i++ ) {
        if ( line[i] == ' ' || line[i] == '\t' || line[i] == '\n' ) {
            line[i] = '\0';
            inWord = false;
        } 
        else if ( !inWord ) {
            words[count++] = &line[i];
            inWord = true;
        }
    }

    // Marking the end of the array
    words[count] = NULL;
    return count;
}


/**
* This function performs the built-in exit command. 
* The words array is the list of pointers to words
* in the user’s command and count is the number of words in the array.
* @param words the words that the command consists of
* @param count the number of arguments in the command line
* @return status the status of exiting
*/
void runExit( char *words[], int count ) {
    // If the amount of words in the command is one, exits the program
    if ( count == 1 ) {
        exit( EXIT_SUCCESS );
    }

    // If the amount of words in the command is not 2 (also not 1 because of previous check), prints that the command is invalid
    if ( count != 2 ) {
        fprintf( stderr, "Invalid command\n" );
        return;
    }

    // Checking that every character is a digit
    for ( int i = 0; words[1][i] != '\0'; i++ ) {
        if ( !isdigit( ( unsigned char ) words[1][i] ) && !( i == 0 && words[1][i] == '-' ) )  {  // The last part is for negative numbers (it was flagging those and then saying they were invalid)
            fprintf( stderr, "Invalid command\n" );
            return;
        }
    }

    // Exiting with the status given by user
    int status = atoi( words[1] );
    exit( status );
    
}


/**
* This function performs the built-in cd command. As with runExit(), the parameters give the
* words in the command entered by the user.
* @param words the words in the command
* @param count the amount of words in the command
*/
void runCd( char *words[], int count ) {

    // If the amount of words in command is 1, prints that the command is invalid
    if (count == 1) {
        fprintf( stderr, "Invalid command\n" );
        return;
    }

    // If the amount of words in command is greater than 2, prints that the command is invalid
    if (count > 2) {
        fprintf( stderr, "Invalid command\n" );
        return;
    }

    // Fails if the direct cannot change to the one specified
    else {
        if ( chdir( words[1] ) != 0 ) {
            fprintf( stderr, "Invalid command\n" );
        }
    }
}


/**
* This function runs a (non-built-in) command by creating a child process and having it call execvp()
* to run the given command.
* @param words the words in the command
* @param count the amount of words in the command
*/
void runCommand( char *words[], int count ) {

    // Forks so that it can run the command
    pid_t pid = fork();

    // If it can't create a child, prints that it can't run the command
    if ( pid == -1 ) {
        fprintf( stderr, "Can't run command %s\n", words[0] );
        return;
    }
    // Runs command and if it can't, prints error and fails
    if ( pid == 0 ) {
        execvp( words[0], words );
        fprintf( stderr, "Can't run command %s\n", words[0] );
        exit( EXIT_FAILURE );
    } 
    // Waits until child is done
    else {
        wait(NULL);
    }
}


/**
* This is the main method for stash.c and can perform various command line operations
* @param argc the amount of arguments on the command line
* @param argv the words in a pointer array form
* @return EXIT_SUCCESS if the method runs successfully / EXIT_FAILURE if it fails
*/
int main(int argc, char *argv[]) {

    char line[MAX_LINE];
    char *words[MAX_WORDS];
    int count;

    // Runs until the user says to exit
    while ( 1 ) {
        printf( "stash> " );
        // Exits the loop when there is an error or EOF
        if ( fgets( line, sizeof(line), stdin ) == NULL ) {
            break;
        }
        // Parses the command
        count = parseCommand( line, words );
        if ( count == 0 ) {
            continue;
        }
        // runs "exit" command if user types it
        if ( strcmp( words[0], "exit" ) == 0 ) {
            runExit( words, count );
        }
        // runs "cd" command if user types it
        else if ( strcmp( words[0], "cd" ) == 0 ) {
            runCd( words, count );
        }
        // runs any other command through the runCommand method
        else {
            runCommand( words, count );
        }
    }

    // Exits successfully if the loop breaks
    return EXIT_SUCCESS;
}
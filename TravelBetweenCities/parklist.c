/**
 * This file creates a parklist using a file. It can sort and print out the parkList
 * @file parklist.c
 * @author Jacob McLain
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "parklist.h"
#include "input.h"
/**
 * This function dynamically allocates storage for the 
 * ParkList and returns a pointer to the new ParkList
 * @return p a pointer to the parkList
 */
ParkList *makeParkList() {
    ParkList *p = malloc(sizeof(ParkList));
    p->capacity = 5;
    p->count = 0;
    p->list = malloc(p->capacity * sizeof(Park));
    return p;
}
/**
 * This function frees the memory used to store the given ParkList, 
 * including freeing space for all the Parks,
 * and freeing space for ParkList struct itself
 * @param parkList a pointer to the parkList
 */
void freeParkList(ParkList *parkList) {
    for(int i = 0; i < parkList->count; i++) {
        free(parkList->list[i].name);
    }
    free(parkList->list);
    free(parkList);
}
/**
 * This function reads all the parks from a park file with the given name. 
 * It makes an instance of the Park struct for each one and stores a 
 * pointer to that Park in the resizable array in the ParkList.
 * void readParks(char const)
 * @param filename a pointer to the file
 * @param parkList a pointer to the parkList
 */
void readParks(char const *filename, ParkList *parkList) {
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    char *str;
    Park p;
    int lineCheck = 0;
    while((str = readLine(fp)) != NULL) {
        if(lineCheck % 2 == 0) {
            if(sscanf(str, " %d %lf %lf ", &p.id, &p.lat, &p.lon) != 3) {   //The first field is an integer that uniquely identifies a park. The second and third fields are double values that represent the park’s latitude and longitude respectively. The second line is a string of up to 40 characters that represents the park name.
                fprintf(stderr, "Invalid park file: %s\n", filename);
                free(str);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            if(parkList->count >= parkList->capacity) {
                parkList->capacity *= 2;
                parkList->list = realloc(parkList->list, parkList->capacity * sizeof(Park));
            }
        }
        else {
            p.name = malloc(strlen(str) + 1);
            strcpy(p.name, str);
            parkList->list[parkList->count++] = p;
            if(strlen(parkList->list[parkList->count - 1].name) > 40 ) {
                fprintf(stderr, "Invalid park file: %s\n", filename);
                free(str);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }
        lineCheck++;
        free(str);
    }
    fclose(fp);
}
/**
 * This function sorts the parks in the given parkList.
 * It uses the qsort() function together with the function
 * pointer parameter to order the parks.
 * @param parkList a pointer to the parkList being sorted
 * @param compare a pointer to a compare function that qsort is using
 */
void sortParks(ParkList *parkList, int (* compare) (void const *va, void const *vb )) {
    qsort(parkList->list, parkList->count, sizeof(Park), compare);
}
/**
 * This function prints all or some of the parks. 
 * It uses the function pointer parameter together 
 * with the string, str, which is passed to the function,
 * to decide which parks to print. This function will be
 * used for the list, list names, and search parks 
 * <search string> commands. 
 * @param parkList a pointer to a parkList
 * @param test a pointer to a test function that determines whether or not str is in the parkList
 * @param str a pointer to a str that is compared with parkList names
 */
void listParks(ParkList *parkList, bool (*test)( Park const *park, char const *str ), char const *str ) {
    printf("%-3s %-40s %8s %8s\n", "ID", "Name", "Lat", "Lon");
    for(int i = 0; i < parkList->count; i++) {
        if(test(&parkList->list[i], str)) {
            printf("%3d %-40s %8.3lf %8.3lf\n", parkList->list[i].id, parkList->list[i].name, parkList->list[i].lat, parkList->list[i].lon);
        }
    }
}
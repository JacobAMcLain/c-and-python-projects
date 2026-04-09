/**
 * This file creates a citylist using a file. It can sort and print out the cityList
 * @file citylist.c
 * @author Jacob McLain
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "citylist.h"
#include "input.h"
/**
 * This function dynamically allocates storage for
 * the CityList, initializes its fields (to store a resizable array)
 * and returns a pointer to the new CityList.
 * It’s kind of like a constructor in Java.
 * @return c a pointer to the cityList
 */
CityList *makeCityList() {
    CityList *c = malloc(sizeof(CityList));
    c->capacity = 5;
    c->count = 0;
    c->list = malloc(c->capacity * sizeof(City));
    return c;
}

/**
 * This function frees the memory used to store
 * the given CityList, including freeing space 
 * for all the Citys, freeing the resizable array 
 * of pointers and freeing space for the CityList struct itself.
 * @param cityList a pointer to the cityList that will be freed
 */
void freeCityList(CityList *cityList) {
    for(int i = 0; i < cityList->count; i++) {
        free(cityList->list[i].name);
    }
    free(cityList->list);
    free(cityList);
}
/**
 * This function reads all the cities from
 * a city file with the given name. It makes
 * an instance of the City struct for each one
 * and stores a pointer to that City in the resizable
 * array in the CityList.
 * @param filename a pointer to the file
 * @param cityList a pointer to the cityList
 */
void readCities(char const *filename, CityList *cityList) {
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    char *str;
    City c;
    while((str = readLine(fp)) != NULL) {
        char shortString[strlen(str) + 1];
        sscanf(str, " %[^0-9]", shortString);
        c.name = malloc(strlen(shortString) + 1);
        strcpy(c.name, shortString);
        if(sscanf(str, "%*[^0-9]%lf %lf", &c.lat, &c.lon) != 2) {
            fprintf(stderr, "Invalid city file: %s\n", filename);
            free(str);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        if(cityList->count >= cityList->capacity) {
            cityList->capacity *= 2;
            cityList->list = realloc(cityList->list, cityList->capacity * sizeof(City));
        }
        cityList->list[cityList->count++] = c;
        if(strlen(cityList->list[cityList->count - 1].name) > 20 ) {
            fprintf(stderr, "Invalid city file: %s\n", filename);
            free(str);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        free(str);
    } 
    fclose(fp);
}
/**
 * This function sorts the cities in the given
 * cityList. It uses the qsort() function together
 * with the function pointer parameter to order the 
 * cities. 
 * @param cityList a pointer to the cityList
 * @param compare a pointer to a compare method that qsort uses
 */
void sortCities(CityList *cityList, int (* compare) (void const *va, void const *vb )) {
    qsort(cityList->list, cityList->count, sizeof(City), compare);
}
/**
 * This function prints all the cities
 * @param cityList the list of cities
 */
void listCities(CityList *cityList) {
    printf("%-20s %8s %8s\n", "Name", "Lat", "Lon");
    for(int i = 0; i < cityList->count; i++) {
        printf("%-20s %8.3lf %8.3lf\n", cityList->list[i].name, cityList->list[i].lat, cityList->list[i].lon);
    }
}

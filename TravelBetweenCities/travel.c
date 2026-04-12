/**
* This is the main file for the whole program. It reads information from two files given by the user.
* It then makes a parkList and a cityList
* It then takes commands from the command line and performs operations to the cityList and parkList
* based on those commands.
* @file travel.c
* @author Jacob McLain
*/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "parklist.h"
#include "citylist.h"
#include "input.h"
/** Multiplier for converting degrees to radians */
#define DEG_TO_RAD ( M_PI / 180 )
/** Radius of the earth in miles. */
#define EARTH_RADIUS 3959.0
/** Used to mark a mileage as invalid */
#define INVALID_MILES 50
/** Used to check if the argument number is invalid */
#define INVALID_ARGUMENTS 4
/** Pointer to target city used in "search parks <city name> <miles>" command */
City *targetCity = NULL;
/** Pointer to target park used in "search cities <park id> <miles>" command */
Park *targetPark = NULL;
/**
 * Computes the distance in miles between two points
 * given their latitudes and longitudes
 * @param lat1 latitude of first point
 * @param lon1 longitude of first point
 * @param lat2 latitude of second point
 * @param lon2 longitude of second point
 * @return distance in miles between the two points
 */
static double distance(double lat1, double lon1, double lat2, double lon2)
{
    // Pretending the center of the earth is at the origin, turn these
    // two locations into unit vectors pointing from the origin.
    double clon1 = cos(lon1 * DEG_TO_RAD);
    double slon1 = sin(lon1 * DEG_TO_RAD);
    double clat1 = cos(lat1 * DEG_TO_RAD);
    double slat1 = sin(lat1 * DEG_TO_RAD);
    double v1[] = { clon1 * clat1, slon1 * clat1, slat1 };
    double clon2 = cos(lon2 * DEG_TO_RAD);
    double slon2 = sin(lon2 * DEG_TO_RAD);
    double clat2 = cos(lat2 * DEG_TO_RAD);
    double slat2 = sin(lat2 * DEG_TO_RAD);
    double v2[] = { clon2 * clat2, slon2 * clat2, slat2 };
    // Dot product these two vectors.
    double dp = v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
    // This should only happen when there are two copies of the same place.
    if ( dp > 1 )
        return 0;
    // Compute the angle between the vectors based on the dot product.
    double angle = acos( dp );
    // Return distance based on the radius of the earth.
    return EARTH_RADIUS * angle;
}
/**
 * Compare function for sortParks based on id
 * @param vaptr a pointer to a city
 * @param vbptr another pointer to a different city
 * @return an int based on how they are sorted
*/
int compareParkId(void const *vaptr, void const *vbptr) {
    Park const *va = vaptr;
    Park const *vb = vbptr;

    if(va->id < vb->id) {
        return -1;
    }

    if(va->id > vb->id) {
        return 1;
    }

    return 0;
}
/**
 * Compare function for sortParks based on distance
 * @param vaptr a pointer to a city
 * @param vbptr another pointer to a different city
 * @return an int based on how they are sorted
*/
int compareParkDist(void const *vaptr, void const *vbptr) {
    Park const *va = vaptr;
    Park const *vb = vbptr;
    if(va->lat < vb->lat) {
        return -1;
    }

    if(va->lat > vb->lat) {
        return 1;
    }

    return 0;
}
/**
 * Compare function for sortParks
 * @param vaptr a pointer to a city
 * @param vbptr another pointer to a different city
 * @return an int based on how they are sorted
 */
int compareParkName(void const *vaptr, void const *vbptr) {
    Park const *va = vaptr;
    Park const *vb = vbptr;
    return strcmp(va->name, vb->name);
}
/**
 * Compare function for sortCities
 * @param vaptr a pointer to a city
 * @param vbptr another pointer to a different city
 * @return an int based on how they are sorted
 */
int compareCityName(void const *vaptr, void const *vbptr) {
    City const *va = vaptr;
    City const *vb = vbptr;
    return strcmp(va->name, vb->name);
}
/**
 * Returns true if the substring is in the Park Name
 * @param vaptr a pointer to a park
 * @param vbptr another pointer to a different park
 * @return returns true or false based on if the str is in the park name
 */
bool test(Park const *park, char const *str) {
    if(!str) {
        return true;
    }
    return strstr(park->name, str) != NULL;
}
/**
 * This is the main method for the whole program. It reads information from two files given by the user.
 * It then makes a parkList and a cityList
 * It then takes commands from the command line and performs operations to the cityList and parkList
 * based on those commands.
 * @param argc the amount of words in the command line - 1
 * @param argv a pointer to a word on the command line
 */
int main(int argc, char *argv[]) {

    if(argc < INVALID_ARGUMENTS) {
        fprintf(stderr, "usage: travel park-file city-file command parameter*\n");
        exit(EXIT_FAILURE);
    }

    int count = 1;
    char *pFileName = argv[count++]; //2  (at the end of execution)
    char *cFileName = argv[count++]; //3
    FILE *pFile;

    if((pFile = fopen(pFileName, "r")) == NULL) {
        fprintf(stderr, "Can't open file: %s\n", pFileName);
        exit(EXIT_FAILURE);
    }

    FILE *cFile;
    if((cFile = fopen(cFileName, "r")) == NULL) {
        fprintf(stderr, "Can't open file: %s\n", cFileName);
        exit(EXIT_FAILURE);
    }

    ParkList *pList = makeParkList();
    readParks(pFileName, pList);
    CityList *cList = makeCityList();
    readCities(cFileName, cList);

    //Check to make sure two parks don't have the same id
    for(int i = 0; i < pList->count; i++) {
        for(int j = i + 1; j < pList->count - 1; j++) {
            if(pList->list[i].id == pList->list[j].id) {
            fprintf(stderr, "Invalid park file: %s\n", pFileName);
            fclose(pFile);
            fclose(cFile);
            freeParkList(pList);
            freeCityList(cList);
            exit(EXIT_FAILURE);
            }
        }
    }

    //Check to make sure two cities don't have the same name
    for(int i = 0; i < cList->count; i++) {
        for(int j = i + 1; j < cList->count; j++) {
            if(strcmp(cList->list[i].name,cList->list[j].name) == 0) {
                fprintf(stderr, "Invalid city file: %s\n", cFileName);
                fclose(pFile);
                fclose(cFile);
                freeParkList(pList);
                freeCityList(cList);
                exit(EXIT_FAILURE);
            }
        }
    }
    //List command Start ***********************************************************
    char *command = argv[count++];

    if(argc == 4 && strcmp(command, "list") == 0) {
        sortParks(pList, compareParkId);     //I dont know if i need to loop through parks or not yet or if i need param for compareParkId
        listParks(pList, test, NULL);        //Parks should be sorted by ID     If list is the only command
    }

    else if(argc == 5 && strcmp(command, "list") == 0) {
        command = argv[count++];

        if(strcmp(command, "names") == 0) {
            sortParks(pList, compareParkName);
            listParks(pList, test, NULL);   //Parks should be sorted by name. If same name, then sorted by ID
        }

        else if(strcmp(command, "cities") == 0) {
            sortCities(cList, compareCityName);
            listCities(cList);                  //Cities should be sorted by their name
        }
    }
    //List command End **************************************************************

    //Search command Start **********************************************************
    else if(argc == 6 && strcmp(command, "search") == 0) {
        command = argv[count++];
        if(strcmp(command, "parks") == 0) {
            sortParks(pList, compareParkId);                 //Parks should be sorted by id and only ones with searched string
            listParks(pList, test, argv[count++]);
        }
    }

    else if(argc == 7 && strcmp(command, "search") == 0) {
        command = argv[count++];
        if(strcmp(command, "parks") == 0) {
            char *cName = argv[count++];
            int cNum = 0;
            int miles = atoi(argv[count++]);
            if(miles < 1) {
                printf("Invalid command\n");
                fclose(pFile);
                fclose(cFile);
                freeParkList(pList);
                freeCityList(cList);
                exit(EXIT_FAILURE);
            }
            //This check to see if they were the same word took FOREVER because of whitespace
            char s1[strlen(cName) + 1];
            char s2[strlen(cName) + 1];
            strncpy(s1, cName, strlen(cName));
            bool check = true;
            for(int i = 0; i < cList->count; i++) {
                strncpy(s2, cList->list[i].name, strlen(cName));
                if(strcmp(s1, s2) == 0) {
                    check = false;
                    cNum = i;
                }
            }
            if(check == true) {
                printf("Invalid command\n");
                fclose(pFile);
                fclose(cFile);
                freeParkList(pList);
                freeCityList(cList);
                exit(EXIT_FAILURE);
            }
            targetCity = &cList->list[cNum];
            printf("%-3s %-40s %6s\n", "ID", "Name", "Miles");
            for(int i = 0; i < pList->count; i++) {
                double pDist = distance(pList->list[i].lat, pList->list[i].lon, targetCity->lat, targetCity->lon);
                if(pDist < miles) {
                    pList->list[i].lat = pDist;  //setting the distance to lat field because distance field DNE
                }                              //and I need to compare it somehow to sort
                else {
                    pList->list[i].lat = miles + INVALID_MILES;
                }
            }
            sortParks(pList, compareParkDist);
            for(int i = 0; i < pList->count; i++) {
                if(pList->list[i].lat < miles) {
                    printf("%3d %-40s %6.2lf\n", pList->list[i].id, pList->list[i].name, pList->list[i].lat); //need to sort parks by distance from city in miles
                }
            }
        }
        else if(strcmp(command, "cities") == 0) {
            int pId = atoi(argv[count++]);
            int pNum = 0;
            int miles = atoi(argv[count++]);
            if(miles < 1) {
                printf("Invalid command\n");
                fclose(pFile);
                fclose(cFile);
                freeParkList(pList);
                freeCityList(cList);
                exit(EXIT_FAILURE);
            }
            printf("%-20s %6s\n", "Name", "Miles");
            for(int i = 0; i < pList->count; i++) {
                if(pList->list[i].id == pId) {
                    pNum = i;
                }
            }
            targetPark = &pList->list[pNum];
            for(int i = 0; i < cList->count; i++) {
                double cDist = distance(cList->list[i].lat, cList->list[i].lon, targetPark->lat, targetPark->lon);
                if(cDist < miles) {
                    printf("%-20s %6.2lf\n", cList->list[i].name, cDist);
                }
            }
        }
    }
    //Search command End ************************************************************
    else {
        fprintf(stderr, "usage: travel park-file city-file command parameter*\n");
        fclose(pFile);
        fclose(cFile);
        freeParkList(pList);
        freeCityList(cList);
        exit(EXIT_FAILURE);
    }
    fclose(pFile);
    fclose(cFile);
    freeParkList(pList);
    freeCityList(cList);
    return EXIT_SUCCESS;
}
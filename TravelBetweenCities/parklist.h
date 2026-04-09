/**
 * This file creates a parklist using a file. It can sort and print out the parkList
 * @file parklist.h
 * @author Jacob McLain
 */
/**
 * Park Struct to have id, name, lat, and lon values
 */
typedef struct {
    int id;
    char *name;
    double lat;
    double lon;
} Park;
/**
 * ParkList struct to have a list of parks, count, and capacity
 */
typedef struct {
    Park *list;
    int count;
    int capacity;
} ParkList;
/**
 * This function dynamically allocates storage for the
 * ParkList and returns a pointer to the new ParkList
 * @return p a pointer to the parkList
 */
ParkList *makeParkList();
/**
 * This function frees the memory used to store the given ParkList,
 * including freeing space for all the Parks,
 * and freeing space for ParkList struct itself
 * @param parkList a pointer to the parkList
 */
void freeParkList(ParkList *parkList);
/**
 * This function reads all the parks from a park file with the given name.
 * It makes an instance of the Park struct for each one and stores a
 * pointer to that Park in the resizable array in the ParkList.
 * void readParks(char const)
 * @param filename a pointer to the file
 * @param parkList a pointer to the parkList
 */
void readParks(char const *filename, ParkList *parkList);
/**
 * This function sorts the parks in the given parkList.
 * It uses the qsort() function together with the function
 * pointer parameter to order the parks.
 * @param parkList a pointer to the parkList being sorted
 * @param compare a pointer to a compare function that qsort is using
 */
void sortParks(ParkList *parkList, int (* compare) (void const *va, void const *vb ));
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
void listParks(ParkList *parkList, bool (*test)( Park const *park, char const *str ), char const *str );
/**
 * This file creates a citylist using a file. It can sort and print out the cityList
 * @file citylist.h
 * @author Jacob McLain
 */
/**
 * City struct that has a name, lat, and long values
 */
typedef struct {
    char *name;
    double lat;
    double lon;
} City;
/**
 * CityList struct to have a list of cities, count, and capacity
 */
typedef struct {
    City *list;
    int count;
    int capacity;
} CityList;
/**
 * This function dynamically allocates storage for
 * the CityList, initializes its fields (to store a resizable array)
 * and returns a pointer to the new CityList.
 * It’s kind of like a constructor in Java.
 * @return c a pointer to the cityList
 */
CityList *makeCityList();
/**
 * This function frees the memory used to store
 * the given CityList, including freeing space
 * for all the Citys, freeing the resizable array
 * of pointers and freeing space for the CityList struct itself.
 * @param cityList a pointer to the cityList that will be freed
 */
void freeCityList( CityList *cityList);
/**
 * This function reads all the cities from
 * a city file with the given name. It makes
 * an instance of the City struct for each one
 * and stores a pointer to that City in the resizable
 * array in the CityList.
 * @param filename a pointer to the file
 * @param cityList a pointer to the cityList
 */
void readCities(char const *filename, CityList *cityList);
/**
 * This function sorts the cities in the given
 * cityList. It uses the qsort() function together
 * with the function pointer parameter to order the
 * cities.
 * @param cityList a pointer to the cityList
 * @param compare a pointer to a compare method that qsort uses
 */
void sortCities(CityList *cityList, int (* compare) (void const *va, void const *vb ));
/**
 * This function prints all the cities
 * @param cityList the list of cities
 */
void listCities(CityList *cityList);
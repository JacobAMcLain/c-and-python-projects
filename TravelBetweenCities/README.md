## TravelBetweenCities (travel.c)

A command-line program written in C that processes geographic data about parks and cities, allowing users to query and analyze locations based on distance and attributes.

### Features

- Loads and validates data from external park and city files
- Builds dynamic lists of parks and cities in memory
- Supports multiple command modes:
  - List all parks or cities
  - Search parks by name substring
  - Find parks near a given city within a radius
  - Find cities near a given park within a radius
- Sorts data using custom comparator functions
- Validates input files for duplicate park IDs and city names
- Handles invalid commands and input errors

### Compile 

```bash
make
```

### Usage

```bash
travel [park-file] [city-file] [command parameter*]
```

# Projects

**This is a collection of Projects I have worked on**

_____________________________________________________________________________________________________________
OUTLINE OF THE PROJECTS

--------------------
## AES (encrypt.c, decrypt.c)

A command-line file encryption and decryption tool written in C that implements the AES (Advanced Encryption Standard) block cipher.
This project includes a full AES implementation along with CLI utilities for encrypting and decrypting binary files using a 128-bit key.

## About

- Full AES-128 block cipher implementation
- Command-line encryption and decryption tools
- Processes binary files in 16-byte blocks
- Supports file-based key input
- Validates input file sizes for correctness
- Encrypts/Decrypts by generating subkeys, then performing numerous matrix operations to encrypt/decrypt binary files

## Usage

**For encrypt:**
./encrypt <key-file> <input-file> <output-file>

**For decrypt:**
./decrypt <key-file> <input-file> <output-file>

--------------------
## Small Python Programs

A Set of 3 Python programs: CPSTracker, Mean_Median_Mode, & MovableBall.

- **CPSTracker**
  - Built an interactive GUI application that measures clicks per second (CPS)
  - Uses a timer-based system to track user input in real time
  - Continuously updates CPS as the user clicks within a defined area
- **Mean_Median_Mode**
  - Command-line program that accepts an arbitrary number of integer inputs from the user
  - Stores inputs in a dynamic list and outputs the mean, median, & mode of the user-created list 
- **MovableBall**
  - Developed a simple interactive graphical program featuring a movable ball
  - Implements keyboard input handling using arrow keys for real-time movement

## Usage

python CPSTracker.py
python Mean_Median_Mode.py
python MovableBall.py

--------------------
## Snark (snark.c)

A command-line archive management system written in C that supports creating, modifying, saving, and inspecting a custom file archive.
The program includes a command interpreter, error-handling system using `setjmp/longjmp`, and support for both interactive and script-based execution.

## About

- Custom command-line interpreter with interactive and script modes
- Archive operations:
  - Add files to archive
  - Extract files from archive
  - Remove files from archive
  - Save archive to disk
  - Load existing archive file
- Supports compressed file storage and on-demand decompression
- Error handling using `setjmp` / `longjmp`

## Usage

./snark [-a archive-file] [-s script-file]

--------------------
## Stash

A lightweight Unix-style shell implemented in C that supports basic command execution.

## About

The shell operates in a continuous loop:

1. Displays a command prompt (`stash>`)
2. Reads user input using `fgets`
3. Parses the input into arguments
4. Checks for built-in commands (`cd`, `exit`)
5. Executes external commands using `fork()` and `execvp()`
6. Waits for the process to finish before continuing

## Usage

./stash

--------------------
## TravelBetweenCities (travel.c)

A command-line program written in C that processes geographic data about parks and cities, allowing users to query and analyze locations based on distance and attributes.

## About

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

## Usage

travel park-file city-file command parameter*

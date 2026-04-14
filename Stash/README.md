## Stash

A Unix-style shell implemented in C that supports basic command execution.

### Features

The shell operates in a continuous loop:

1. Displays a command prompt (`stash>`)
2. Reads user input using `fgets`
3. Parses the input into arguments
4. Checks for built-in commands (`cd`, `exit`)
5. Executes external commands using `fork()` and `execvp()`
6. Waits for the process to finish before continuing

### Compile

```bash
gcc -Wall -std=c99 -o stash stash.c
```

### Usage

```bash
./stash
```

## Snark (snark.c)

A command-line archive management system written in C that supports creating, modifying, saving, and inspecting a custom file archive.
The program includes a command interpreter, error-handling system using `setjmp/longjmp`, and support for both interactive and script-based execution.

### Features

- Custom command-line interpreter with interactive and script modes
- Archive operations:
  - Add files to archive
  - Extract files from archive
  - Remove files from archive
  - Save archive to disk
  - Load existing archive file
- Supports compressed file storage and on-demand decompression
- Error handling using `setjmp` / `longjmp`

### Compile 

```bash
make
```

### Usage

```bash
./snark [-a archive-file] [-s script-file]
```

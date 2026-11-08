# minishell-42

A lightweight Unix shell project written in C, inspired by `bash`, built as part of the 42 curriculum.

This project implements a simplified shell loop that reads user input, tokenizes commands, parses redirections and pipes, and executes external programs using `fork()` and `execve()`.

## Features

- Interactive shell prompt using `readline`
- Command tokenization for:
  - words and arguments
  - `|` pipes
  - `<`, `>`, `>>`, `<<` redirections
  - quoted strings
- Parsing into a command structure with arguments and redirections
- Execution of external commands from `PATH`
- Basic redirection support for input/output/appending
- Support for chained commands via pipes

## Project structure

- `main.c` — shell loop and entry point
- `mk_token.c` — token generation and input splitting
- `parc_token.c` — command parsing and redirection handling
- `execute.c` — execution logic with redirections and process spawning
- `minishell.h` — shared structures and function declarations
- `makefile` — build script
- `LIBFT/` — custom C library dependency

## Build

Requirements:

- `gcc`
- `make`
- `readline` development library

Run:

```bash
make
```

This creates the executable:

```bash
./minishell
```

## Usage

Once the shell starts, you can type commands such as:

```bash
ls -l
cat file.txt
grep hello test.txt
echo hello > output.txt
cat < input.txt
ls | grep minishell
```

## Notes

This is a minimal minishell implementation and does not yet cover every bash feature. In particular, it is a simplified shell focused on core parsing and execution behavior rather than full shell compatibility.

## Author

Project developed in the context of 42 school.

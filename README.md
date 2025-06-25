# Minishell

A small Unix shell developed as part of the [42 School](https://www.42.fr/) curriculum. Minishell is written in C and mimics many features of `bash`, providing a great learning experience on how shells work under the hood.

![screenshot](path/to/demo.png)

## Features

- **Command execution** with absolute or relative paths
- **Environment variables** with full expansion inside commands
- **Wildcard expansion (`*`)** similar to `bash`
- **Redirections**: `>`, `>>`, `<`, `<<` (heredoc)
- **Pipelines** using `|`
- **Logical operators**: `&&` and `||`
- **Subshells** through parentheses `( )`
- **Built‑ins**: `cd`, `echo`, `export`, `unset`, `exit`, `env`, `pwd`
- **Heredoc** with optional variable expansion
- **Signal handling** for `SIGINT`, `SIGQUIT` and others
- **Abstract Syntax Tree (AST)** for parsing and execution
- **Robust tokenizer** with single and double quote support

## Project Structure

```
.
├── Makefile       # build instructions
├── include/       # header files
├── libft/         # custom C library used by the project
├── src/           # source code
│   ├── ast/       # AST building and traversal
│   ├── builtin/   # built‑in command implementations
│   ├── env/       # environment variable utilities
│   ├── executor/  # command execution engine
│   ├── expansion/ # variable and wildcard expansion
│   ├── heredoc/   # heredoc support
│   ├── prompt/    # prompt generation
│   └── ...        # other helpers
└── README.md
```

## Compilation

Minishell requires the GNU readline library. Run:

```bash
make
```

The resulting binary will be named `minishell`.

## Usage

Launch the shell with:

```bash
./minishell
```

Once running, you can execute commands as you would in `bash`, including pipelines, redirections and variable expansion.

## Testing

- **Compare with bash**: run the same commands in `bash` and Minishell to verify identical behavior.
- **Memory leaks**: use `valgrind` via `make leak` or `make leakfile`.
- **42_minishell_tester**: you can use community test suites to run automated tests.

## Challenges

During development we faced several tricky areas:

1. **Parsing** – building an AST that supports subshells, logical operators and nested redirections required a carefully designed tokenizer and parser.
2. **Signals** – reproducing `bash` signal behavior (especially in heredocs and pipelines) demanded fine‑grained signal handling and a global status variable.
3. **Wildcards and variable expansion** – matching `bash` behavior while keeping the code maintainable took several iterations.

## Team

This project was created by **Marcus Gomes** and **Caio de Souza Marinho**.

## License

This project is released under the [MIT License](LICENSE).


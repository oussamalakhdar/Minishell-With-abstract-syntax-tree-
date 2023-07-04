# Minishell-With-abstract-syntax-tree-

Minishell
This project is a recreation of a bash-like shell in C, using an abstract syntax tree (AST).

Features
cd; pwd; echo; export; unset; env; exit; CTRL-C; CTRL-; CTRL-D; | pipes; ; semicolons; > >> < redirections; && || operators; local variable; env expansions + $?
Line editing (← / →)
History (↑ / ↓)
Copy (start: CTRL + K | stop : CTRL + K)
Paste (CTRL + P)
Cut (start: CTRL + K | stop : CTRL + L)
Usage
Compile the mandatory part
make

Compile with bonus
make bonus

Launch the shell
./minishell

Abstract Syntax Tree
The AST is used to represent the syntactic structure of the commands entered by the user. This allows the shell to parse and execute commands more efficiently.

How to contribute

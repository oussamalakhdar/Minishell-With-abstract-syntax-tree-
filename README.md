# Minishell-With-abstract-syntax-tree-

Minishell is a simplified version of a **shell** or **command-line** interface implemented in **C**. It provides basic shell functionality, including:

Executing commands: **Minishell** can execute any command that can be executed from the command line, including built-in commands, such as **cd** and **pwd, and external commands, such as **ls** and **grep**.
Handling input/output redirection: Minishell can handle input/output redirection, which allows you to redirect the input or output of a command to a file or another command. For example, you could use **>>** to append the output of a command to a file, or **|** to pipe the output of one command to the input of another command.
Managing background processes: Minishell can manage background processes, which allows you to run commands in the background so that you can continue to use the shell while the commands are running.
Implementing built-in commands: Minishell can implement built-in commands, which are commands that are executed by the shell itself. This allows you to create custom commands that are specific to your shell.

## All the Features:

- Display a prompt when waiting for a new command.
- Working history functionality.
- Search and launch the right executable (based on the **PATH** variable or using a relative or absolute path).
- No more than one global variable.
- No interpretation of unclosed quotes or unnecessary special characters.
- Handling of single quotes **(')**, preventing interpretation of metacharacters in the quoted sequence.
- Handling of double quotes **(")** except for **$** (dollar sign) metacharacter.
- Redirections:
  - `<` redirects input.
  - `>` redirects output.
  - `<<` reads input until a line containing the delimiter is seen.
  - `>>` redirects output in append mode.
- Pipes `|` for command pipelines.
- Handling of environment variables (`$` followed by a sequence of characters) for expansion.
- Handling of `$?` for expanding the exit status of the most recently executed foreground pipeline.
- Handling of **ctrl-C**, **ctrl-D**, and **"ctrl-\\"** signals.
- Interactive mode behavior:
  - **ctrl-C**, displays a new prompt on a new line.
  - **ctrl-D** exits the shell.
  - **"ctrl-\\"** does nothing.
- Built-in commands:
  - `echo` with option `-n`.
  - `cd` with only a relative or absolute path.
  - `pwd` with no options.
  - `export` with no options.
  - `unset` with no options.
  - `env` with no options or arguments.
  - `exit` with no options.

Here are some additional features that minishell can support:

Line editing: Minishell can support line editing, which allows you to edit the command line before it is executed. This can be useful for correcting mistakes or for entering long commands.
History: Minishell can maintain a history of the commands that have been executed. This can be useful for recalling commands that you have used previously.
Signal handling: Minishell can handle signals, which are events that can interrupt the execution of a program. This allows minishell to respond to events such as Ctrl-C and Ctrl-Z.**

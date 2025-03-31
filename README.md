# Pico Shell
Pico Shell is a simple command-line shell written in C. It allows users to run basic commands and execute external programs.

## Features
#### Supports basic built-in commands:

*echo* → Prints text to the screen.

*pwd* → Shows the current working directory.

*cd* <directory> → Changes the current directory.

*exit* → Closes the shell.

#### Executes external commands using execvp.

#### Runs commands in a child process using fork.

## How It Works
#### The shell waits for user input.

#### It splits the input into words.

#### If the command is built-in, it runs inside the shell.

#### If the command is external, the shell creates a child process to execute it.

#### If the command does not exist, it prints an error message.

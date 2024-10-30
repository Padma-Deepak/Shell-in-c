# Shell-in-c


---

# Documentation for SimpleShell

## Introduction
Welcome to the documentation for my project, **SimpleShell**! I decided to build this shell as a  way to learn more about how command-line interfaces work and how they interact with the operating system. So, let me walk you through what it does and how you can use it.

## What’s SimpleShell All About?
SimpleShell is a basic command-line interface that lets you run commands and navigate your file system. It’s got some essential built-in commands to help you get around and manage your workflow without any fuss.

## What Can You Do with SimpleShell?
Here are the features I included:

1. **Built-in Commands**:
   - **`cd`**: Change directories like a pro.
   - **`pwd`**: Print your current working directory—no more guessing where you are!
   - **`help`**: Get the lowdown on how to use the shell.
   - **`exit`**: Close the shell when you’re done.
   - **`history`**: Check out a list of your recent commands—great for recalling what you did last!

2. **Command History**: SimpleShell remembers the last 100 commands you entered. It’s super handy for when you want to repeat something without typing it all over again.

3. **Running External Programs**: You can execute external commands by firing up a new process, which makes the experience feel more like a traditional shell.

## How’s the Code Structured?
The code has a straightforward flow that keeps running until you decide to exit. Here’s a peek at the important functions:

- **`main()`**: This is where the magic begins! It sets everything up, prompts for input, saves commands to history, and executes them.
- **`read_line()`**: Reads your input from the terminal and allocates memory for it. 
- **`split_line()`**: Breaks your input into tokens so the shell can understand what you’re trying to do.
- **`execute_command()`**: Checks if your command is built-in or an external one and then gets it running.
- **`launch_process()`**: This is the part that actually creates a new process to run external commands.
- **`change_directory()`**: Changes your current directory with a simple command.
- **`print_working_directory()`**: Shows you where you are in the file system.
- **`display_help()`**: Gives you the scoop on using SimpleShell and its commands.
- **`exit_shell()`**: Cleans up when you’re done and exits the shell.

## How to Use SimpleShell
Here’s how to get started with SimpleShell:

1. First, compile the code using a C compiler:
   ```bash
   gcc -o SimpleShell shell.c
   ```

2. Run the compiled shell:
   ```bash
   ./SimpleShell
   ```

3. You’ll see the prompt (`SimpleShell>`), and you can start typing commands. Here are some examples:
   - To change to the parent directory:
     ```
     SimpleShell> cd ..
     ```
   - To see your current directory:
     ```
     SimpleShell> pwd
     ```
   - To view your command history:
     ```
     SimpleShell> history
     ```
   - If you need help, just type:
     ```
     SimpleShell> help
     ```
   - And when you’re ready to exit:
     ```
     SimpleShell> exit
     ```

--- 

## README.md for POSIX Shell Implementation

### Project Overview

This project is a POSIX-compliant shell implemented in C++. The shell can execute system commands, manage processes, handle input/output redirection, and support piping. It also includes several commands like `cd`, `echo`, `pwd`, and `ls` with basic functionality.

### Specifications

1. **Custom Shell Prompt:**
   - Displays the shell prompt with the format: `<username>@<system_name>:<current_directory>~>`.
   - The prompt reflects changes in the working directory and handles random spaces and tabs.

2. **Commands:**
   - **`cd`**: Change the current working directory. Supports flags like `.`, `..`, `-`, and `~`.
   - **`echo`**: Outputs the given string to the terminal, preserving spaces inside quotes.
   - **`pwd`**: Prints the current working directory.
   - **`ls`**: Lists directory contents with flags `-a`, `-l`,`-al` and `-la`. It can be executed with zero or more directories as parameters.

3. **System Commands:**
   - Supports running system commands in the foreground or background.
   - Background processes are indicated by the process ID (PID).

4. **Process Management:**
   - `pinfo` command provides information about a the current process if no process Id is passed.

5. **I/O Redirection:**
   - Supports input (`<`), output (`>`), and append (`>>`) redirection.
   - Handles cases where files are created or overwritten.

6. **Pipelines:**
   - Supports multiple pipelines, allowing the output of one command to be used as input for another.

7. **Autocomplete:**
   - Provides autocomplete functionality for files and directories present in current working directory as well as for commands using the TAB key.

8. **History:**
   - Implements a history feature that stores up to 20 commands across sessions, with the ability to navigate and execute previous commands using the `UP` arrow key and `history` command.

### File Structure

- **`main.cpp`**: The entry point of the shell. Handles the command prompt and parses user input.
- **`changeDir.cpp`**: Implements built-in command `cd`.
- **`listContents.cpp`**: Implements the `ls` command and handles file listing with appropriate flags.
- **`ioRedirection.cpp`**: Implements input/output redirection logic.
- **`pipe.cpp`**: Implements piping functionality for commands.
- **`termInput.cpp`**: Implements the autocomplete feature using the `TAB` key, backspace.
- **`printHistory.cpp`**: Manages command history and handles the UP arrow key for command navigation.
- **`makefile`**: Automates the build process for the project.
- **`creTokens.cpp`**: Used for tokenization of input strings using strtok().
- **`echo.cpp`**: Implements built-in command `echo`.
- **`getCurrDirPath.cpp`**: Implements built-in command `pwd`.
- **`search.cpp`**: Implements command `search`, it will search in current directory and the one level down directory.
- **`processInfo.cpp`**: Implements command `pinfo`, with and without Pid.

### Compilation and Execution

1. **Compilation:**
   - Use the provided `makefile` to compile the project.
   - Run the command:
     ```bash
     make
     ```

2. **Execution:**
   - Run the shell using the following command:
     ```bash
     make run
     ```

### Notes

- **Error Handling:** The shell handles errors like invalid commands, incorrect usage of built-in commands, and non-existent files for redirection.
- **Memory Management:** The shell is designed to handle memory efficiently, ensuring no leaks occur.

### References

- POSIX documentation for system calls and signals.
- `man` pages for detailed usage of various system calls and utilities.

### Acknowledgments

This project was developed as part of the Advanced Operating Systems course at [Your Institution]. Special thanks to the course instructors and teaching assistants for their guidance and support.

---

This README provides a brief overview of the shell project, its features, file structure, and instructions for compiling and running the shell.
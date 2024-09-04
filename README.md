## README.md for POSIX Shell Implementation

### Project Overview

This project is a POSIX-compliant shell implemented in C++. The shell can execute system commands, manage processes, handle input/output redirection, and support piping. It also includes several built-in commands like `cd`, `echo`, `pwd`, and `ls` with basic functionality.

### Features

1. **Custom Shell Prompt:**
   - Displays the shell prompt with the format: `<username>@<system_name>:<current_directory> >`.
   - The prompt reflects changes in the working directory and handles random spaces and tabs.

2. **Built-in Commands:**
   - **`cd`**: Change the current working directory. Supports flags like `.`, `..`, `-`, and `~`.
   - **`echo`**: Outputs the given string to the terminal, preserving spaces inside quotes.
   - **`pwd`**: Prints the current working directory.
   - **`ls`**: Lists directory contents with flags `-a` and `-l`.

3. **System Commands:**
   - Supports running system commands in the foreground or background.
   - Background processes are indicated by the process ID (PID).

4. **Process Management:**
   - Handles foreground and background processes.
   - `pinfo` command provides information about a specific process.

5. **I/O Redirection:**
   - Supports input (`<`), output (`>`), and append (`>>`) redirection.
   - Handles cases where files are created or overwritten.

6. **Pipelines:**
   - Supports command pipelines, allowing the output of one command to be used as input for another.

7. **Signal Handling:**
   - Supports simple signal handling, such as `CTRL-C` to interrupt processes and `CTRL-Z` to suspend them.

8. **Autocomplete:**
   - Provides autocomplete functionality for file and directory names using the TAB key.

9. **History:**
   - Implements a history feature that stores up to 20 commands across sessions, with the ability to navigate and execute previous commands using the UP arrow key.

### File Structure

- **`main.cpp`**: The entry point of the shell. Handles the command prompt and parses user input.
- **`builtins.cpp`**: Implements built-in commands like `cd`, `echo`, and `pwd`.
- **`listContents.cpp`**: Implements the `ls` command and handles file listing with appropriate flags.
- **`processManagement.cpp`**: Manages process creation, handling, and termination.
- **`redirection.cpp`**: Implements input/output redirection logic.
- **`piping.cpp`**: Implements piping functionality for commands.
- **`signalHandling.cpp`**: Handles signals like `CTRL-C`, `CTRL-Z`, and `CTRL-D`.
- **`autocomplete.cpp`**: Implements the autocomplete feature using the TAB key.
- **`history.cpp`**: Manages command history and handles the UP arrow key for command navigation.
- **`makefile`**: Automates the build process for the project.
  
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
     ./shell
     ```

### Notes

- **Error Handling:** The shell handles errors like invalid commands, incorrect usage of built-in commands, and non-existent files for redirection.
- **Memory Management:** The shell is designed to handle memory efficiently, ensuring no leaks occur.

### Limitations

- The shell does not support advanced redirection operators like `2>&1`, `&>`, `>&`, or `2>`.
- Autocomplete does not handle files or directories with spaces in their names.
- The shell does not use environment variables `OLDPWD` or `PWD`.

### Future Improvements

- Extend the `ls` command to handle more advanced flags.
- Improve the autocomplete feature to handle more complex cases.
- Implement additional built-in commands and enhance existing ones.

### References

- POSIX documentation for system calls and signals.
- `man` pages for detailed usage of various system calls and utilities.

### Acknowledgments

This project was developed as part of the Advanced Operating Systems course at [Your Institution]. Special thanks to the course instructors and teaching assistants for their guidance and support.

---

This README provides a brief overview of the shell project, its features, file structure, and instructions for compiling and running the shell.
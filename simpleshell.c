#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h> // Include for getcwd

#define BUFFER_SIZE 1024
#define TOKEN_BUFFER 64
#define TOKEN_DELIMITERS " \t\r\n\a"
#define MAX_HISTORY 100

// Global variable for command history
char *history[MAX_HISTORY];
int history_count = 0;

// Function declarations
void add_to_history(const char *line);
void print_history();
char *read_line();
char **split_line(char *line);
int execute_command(char **args);
int launch_process(char **args);
int change_directory(char **args);
int print_working_directory(char **args);
int display_help(char **args);
int exit_shell(char **args);
int num_builtins();

// Built-in commands
char *builtin_str[] = {
    "cd",
    "pwd",
    "help",
    "exit",
    "history"
};

int (*builtin_func[]) (char **) = {
    &change_directory,
    &print_working_directory,
    &display_help,
    &exit_shell,
    &print_history
};

int main() {
    char *line;
    char **args;
    int status;

    do {
        printf("SimpleShell> ");
        line = read_line();
        add_to_history(line);
        args = split_line(line);
        status = execute_command(args);

        free(line);
        free(args);
    } while (status);

    return EXIT_SUCCESS;
}

void add_to_history(const char *line) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(line);
    } else {
        free(history[0]);
        memmove(history, history + 1, (MAX_HISTORY - 1) * sizeof(char *));
        history[MAX_HISTORY - 1] = strdup(line);
    }
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}

char *read_line() {
    int bufsize = BUFFER_SIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "SimpleShell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= bufsize) {
            bufsize += BUFFER_SIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "SimpleShell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **split_line(char *line) {
    int bufsize = TOKEN_BUFFER, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "SimpleShell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIMITERS);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOKEN_BUFFER;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "SimpleShell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

int execute_command(char **args) {
    if (args[0] == NULL) {
        return 1; // Empty command
    }

    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return launch_process(args);
}

int launch_process(char **args) {
    PROCESS_INFORMATION process_info;
    STARTUPINFO startup_info;

    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    ZeroMemory(&process_info, sizeof(process_info));

    // Create the child process
    if (!CreateProcess(
            NULL,           // No module name (use command line)
            args[0],       // Command line
            NULL,          // Process handle not inheritable
            NULL,          // Thread handle not inheritable
            FALSE,         // Set handle inheritance to FALSE
            0,             // No creation flags
            NULL,          // Use parent's environment block
            NULL,          // Use parent's starting directory
            &startup_info, // Pointer to STARTUPINFO structure
            &process_info) // Pointer to PROCESS_INFORMATION structure
       ) {
        fprintf(stderr, "Error creating process: %d\n", GetLastError());
        return -1;
    }

    // Wait until child process exits
    WaitForSingleObject(process_info.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    return 1;
}

int change_directory(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "SimpleShell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("SimpleShell");
        }
    }
    return 1;
}

int print_working_directory(char **args) {
    char cwd[BUFFER_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("SimpleShell");
    }
    return 1;
}

int display_help(char **args) {
    printf("SimpleShell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built-in commands:\n");

    for (int i = 0; i < num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int exit_shell(char **args) {
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
    return 0; // Exit the shell
}

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}


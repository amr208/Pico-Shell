#include <stdio.h>      // Standard input/output functions
#include <string.h>     // String handling functions
#include <unistd.h>     // POSIX API for system calls (fork, exec, chdir, etc.)
#include <sys/wait.h>   // Wait for child processes
#include <sys/types.h>  // Data types used in system calls
#include <sys/stat.h>   // File status functions
#include <fcntl.h>      // File control options
#include <stdlib.h>     // General purpose functions (memory allocation, exit, etc.)

#define BUF_SIZE 100          // Maximum size of the input buffer
#define LINUX_PWD_MAX 4096    // Maximum size of the current working directory path
#define ECHO_MAX_WORDS 20     // Maximum number of words in a command

int main() {
    char input_buf[BUF_SIZE];       // Buffer to store user input
    char CWD_buf[LINUX_PWD_MAX];    // Buffer to store the current working directory
    char *token;                    // Pointer for tokenizing input
    int echo_words_counter = 0;      // Counter for words in input command
    char *passed_args[ECHO_MAX_WORDS];       // Array to store command and its arguments
    char *ext_passed_args[ECHO_MAX_WORDS - 1];  // Array to store command arguments excluding the command itself
    char **passed_args_ptr = passed_args;    // Pointer to passed_args array

    while (1) {
        echo_words_counter = 0;    // Reset word counter for each command input

        // Display shell prompt with current working directory
        printf("Femto Shell: %s >> ", getcwd(CWD_buf, sizeof(CWD_buf) - 1));

        fgets(input_buf, BUF_SIZE, stdin);  // Read user input
        input_buf[strlen(input_buf) - 1] = 0;  // Remove newline character from input

        if (strlen(input_buf) == 0)  // If input is empty, restart loop
            continue;

        // Clear ext_passed_args array to avoid leftovers from previous commands
        for (int j = 0; j < sizeof(ext_passed_args) / sizeof(ext_passed_args[0]); j++) {  
            ext_passed_args[j] = NULL;
        }

        input_buf[strcspn(input_buf, "\n")] = '\0';  // Ensure input has no trailing newline

        // Tokenize input into words
        token = strtok(input_buf, " ");
        while (token != NULL) {
            passed_args[echo_words_counter] = token;  // Store each token (word) in passed_args array
            ext_passed_args[echo_words_counter] = token;  // Store arguments excluding the command itself
            token = strtok(NULL, " ");  // Get next token
            echo_words_counter++;  // Increment word counter
        }
        ext_passed_args[echo_words_counter] = NULL;  // Ensure last argument is NULL for exec functions

        // Check for built-in commands
        if (strcmp(passed_args[0], "echo") == 0) {  // Handle 'echo' command
            for (int i = 1; i < echo_words_counter; i++) {  // Print all words after 'echo'
                printf("%s ", passed_args[i]);
            }
            echo_words_counter = 0;  // Reset counter
            printf("\n");  // Print newline after output
        } 
        else if (strcmp(passed_args[0], "exit") == 0) {  // Handle 'exit' command
            printf("Good Bye :)\n");  // Print exit message
            break;  // Terminate shell
        } 
        else if (strcmp(passed_args[0], "pwd") == 0) {  // Handle 'pwd' command
            printf("%s\n", getcwd(CWD_buf, sizeof(CWD_buf) - 1));  // Print current directory
        } 
        else if (strcmp(passed_args[0], "cd") == 0) {  // Handle 'cd' command
            if (chdir(passed_args[1]) == -1)  // Change directory, check if successful
                printf("Usage: cd /path\n");  // Print error message if invalid path
        } 
        else {  // If not a built-in command, execute as external command
            if (fork() != 0) {  // Parent process
                wait(NULL);  // Wait for child process to finish
            } 
            else {  // Child process
                execvp(passed_args[0], ext_passed_args);  // Execute command with arguments

                // If execvp fails, print error and exit child process
                printf("Cannot find command!!\n");  
                exit(1);  
            }
        }
    }
    return 0;  // End of program
}

#include "loop.h"
#include "book.h"

void process_command(const char *command, const char *ordering_strategy, const char *filename, struct Index **bookIndexArray, size_t *size) {
    if (strncmp(command, "add ", 4) == 0) {
        // Receive book info and add it to the binary db
        addBook(command,filename,bookIndexArray,size);
        printf("exit\n");
    } else if (strcmp(command, "printData") == 0) {
        // Read and print data from the binary file
        read_and_print_data(filename);
    } else if (strcmp(command, "printInd") == 0) {
        // Read and print data from the binary file
        printBookInfoArray(*bookIndexArray,*size);
        printf("exit\n");
    } else if (strcmp(command, "printRec") == 0) {
        // Read and print data from the binary file
        printRec(filename,*bookIndexArray,*size);
        printf("exit\n");
    } else if (strcmp(command, "printAll") == 0) {
        // Read and print data from the binary file
        read_and_print_data(filename);
        printf("exit\n");
    } else if (strcmp(command, "exit") == 0) {
        // Inform the user that the program will exit
        printf("all done\n");
    } else {
        // Handle unrecognized commands
        printf("Unrecognized command: %s\n", command);
    }
}

void loop(const char *ordering_strategy, const char *filename, struct Index **bookIndexArray, size_t *size) {
    /** Buffer to store user input **/
    char command_buffer[100];

    while (1) {
        /** Read user input **/
        if (fgets(command_buffer, sizeof(command_buffer), stdin) == NULL) {
            perror("Error reading input");
            return;
        }

        /** Remove newline character from input **/
        command_buffer[strcspn(command_buffer, "\n")] = '\0';

        /** Process the entered command **/
        process_command(command_buffer, ordering_strategy, filename,bookIndexArray, size);

        /** Check if the user entered the "exit" command **/
        if (strcmp(command_buffer, "exit") == 0) {
            break; /** Exit the loop **/
        }
    }
}

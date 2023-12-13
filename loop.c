#include "loop.h"
#include "book.h"

void saveBookIndexArray(const char *filename, const struct Index *array, size_t size) {
    const char *extension = ".ind";
    const char *mode = "wb";

    // Open or create the file with the specified extension and mode
    FILE *file = openFileWithExtension(filename, extension, mode);

    if (!file) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    // Write each BookInfo entry to the file
    for (size_t i = 0; i < size; i++) {
        fwrite(&array[i], sizeof(struct Index), 1, file);
    }

    fclose(file);
}

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
    } else if (strncmp(command, "find ", 5) == 0) {
        // Read and print data from the binary file
        findBook(command,filename,*bookIndexArray,*size);
        printf("exit\n");
    } else if (strcmp(command, "exit") == 0) {
        // Inform the user that the program will exit
        saveBookIndexArray(filename,*bookIndexArray,*size);
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

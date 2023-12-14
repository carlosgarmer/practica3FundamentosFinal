#include "loop.h"
#include "book.h"

struct Index* readBookIndexArray(const char *filename, size_t *outSize) {
    const char *extension = ".ind";
    const char *mode = "rb";

    // Open or create the file with the specified extension and mode
    FILE *file = openFileWithExtension(filename, extension, mode);

    if (!file) {
       return NULL;
    }

    // Calculate the size of the file by seeking to the end and getting the position
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    if (fileSize <= 0) {
        // File is empty or has an issue, return NULL
        fclose(file);
        return NULL;
    }

    // Calculate the number of entries in the array
    size_t arraySize = fileSize / sizeof(struct Index);

    // Allocate memory for the array
    struct Index *array = (struct Index*)malloc(arraySize * sizeof(struct Index));

    if (!array) {
        perror("Memory allocation error");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Read each BookInfo entry from the file
    fseek(file, 0, SEEK_SET);  // Move back to the beginning of the file
    fread(array, sizeof(struct Index), arraySize, file);

    fclose(file);

    // Set the output size
    *outSize = arraySize;

    return array;
}

struct Deleted *readDeletedArray(const char *filename, size_t *outSize) {
    const char *extension = ".lst";
    const char *mode = "rb";

    // Open the file with the specified extension and mode
    FILE *file = openFileWithExtension(filename, extension, mode);

    if (!file) {
        // File does not exist or cannot be opened
        return NULL;
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size == 0) {
        fclose(file);
        return NULL; // File is empty
    }

    // Move the file position back to the beginning
    fseek(file, 0, SEEK_SET);

    // Read ordering strategy (assuming it is an int)
    int ordering_strategy;
    fread(&ordering_strategy, sizeof(int), 1, file);

    // Read the Deleted array
    fseek(file, sizeof(int), SEEK_SET); // Move to the beginning of the Deleted array
    *outSize = (file_size - sizeof(int)) / sizeof(struct Deleted);

    // Allocate memory for the Deleted array
    struct Deleted *result = malloc(*outSize * sizeof(struct Deleted));

    if (!result) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Read the Deleted array from the file
    fread(result, sizeof(struct Deleted), *outSize, file);

    fclose(file);

    return result;
}

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

void saveDeletedArray(const char *filename, const char *ordering_strategy, const struct Deleted *array, size_t size) {
    const char *extension = ".lst";
    const char *mode = "wb";

    // Open or create the file with the specified extension and mode
    FILE *file = openFileWithExtension(filename, extension, mode);

    if (!file) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }


    if (strcmp(ordering_strategy, "worst_fit") == 0 ){
    int worst_fit_value = WORSTFIT;
    fwrite(&worst_fit_value, sizeof(int), 1, file);
    }
    if (strcmp(ordering_strategy, "first_fit") == 0 ){
        int first_fit_value = FIRSTFIT;
        fwrite(&first_fit_value, sizeof(int), 1, file);
    }
    if (strcmp(ordering_strategy, "best_fit") == 0 ){
        int best_fit_value = BESTFIT;
        fwrite(&best_fit_value, sizeof(int), 1, file);
    }

    // Write each BookInfo entry to the file
    for (size_t i = 0; i < size; i++) {
        fwrite(&array[i], sizeof(struct Deleted), 1, file);
    }

    fclose(file);
}

void process_command(const char *command, const char *ordering_strategy, const char *filename, struct Index **bookIndexArray, size_t *size,struct Index **bookDeletedArray, size_t *deletedSize){
    if (strncmp(command, "add ", 4) == 0) {
        // Receive book info and add it to the binary db
        addBook(command,filename,bookIndexArray,size);
        printf("exit\n");
    } else if (strncmp(command, "del ", 4) == 0) {
        // Delete book info and add it to the binary db
        deleteBook(command,filename,ordering_strategy,bookIndexArray,size,bookDeletedArray,deletedSize);
        printf("exit\n");
    } else if (strcmp(command, "printData") == 0) {
        // Read and print data from the binary file
        read_and_print_data(filename);
    } else if (strcmp(command, "printInd") == 0) {
        // Read and print data from the binary file
        printBookIndexArray(*bookIndexArray,*size);
        printf("exit\n");
    } else if (strcmp(command, "printRec") == 0) {
        // Read and print data from the binary file
        printRec(filename,*bookIndexArray,*size);
        printf("exit\n");
    } else if (strcmp(command, "printLst") == 0) {
        // Read and print data from the binary file
        printDeletedArray(*bookDeletedArray,*deletedSize);
        printf("exit\n");
    } else if (strncmp(command, "find ", 5) == 0) {
        // Read and print data from the binary file
        findBook(command,filename,*bookIndexArray,*size);
        printf("exit\n");
    } else if (strcmp(command, "exit") == 0) {
        // Inform the user that the program will exit
        saveBookIndexArray(filename,*bookIndexArray,*size);
        saveDeletedArray(filename,ordering_strategy,*bookDeletedArray,*deletedSize);
        printf("all done\n");
    } else {
        // Handle unrecognized commands
        printf("Unrecognized command: %s\n", command);
    }
}

void loop(const char *ordering_strategy, const char *filename,struct Index **bookIndexArray, size_t *size,struct Index **bookDeletedArray, size_t *deletedSize) {
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
        process_command(command_buffer, ordering_strategy, filename,bookIndexArray, size, bookDeletedArray, deletedSize);

        /** Check if the user entered the "exit" command **/
        if (strcmp(command_buffer, "exit") == 0) {
            break; /** Exit the loop **/
        }
    }
}

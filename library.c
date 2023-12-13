#include "loop.h"
#include "struct.h"
#include "book.h"

/**
 * Main function that accepts two command-line arguments:
 * @param argc: Number of command-line arguments
 * @param argv: Array of command-line argument strings
 * @return: Exit status
 **/
struct Index* readBookInfoArray(const char *filename, size_t *outSize) {
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

int main(int argc, char *argv[]) {
    /** Check if the correct number of arguments is provided **/
    if (argc != 3) {
        fprintf(stdout, "Missing argument\n", argv[0]);
        return 0; /** Return error code if incorrect arguments **/
    }

    /** Extract the arguments **/
    const char *ordering_strategy = argv[1];
    const char *filename = argv[2];

    if ( !(strcmp(ordering_strategy, "first_fit") == 0 || strcmp(ordering_strategy, "best_fit") == 0 || strcmp(ordering_strategy, "worst_fit") == 0)) {
       fprintf(stdout, "Unknown search strategy unknown_search_strategy\n", ordering_strategy);
       return 0; /** Return error code if incorrect arguments **/
    }

    printf("Type command and argument/s.\n");
    printf("exit\n");

    size_t arraySize = 0;
    struct Index *bookIndexArray = readBookInfoArray(filename,&arraySize);
    

    loop(ordering_strategy, filename,&bookIndexArray, &arraySize);

    return 0; /** Return success code **/
}

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

    size_t arraySize;

    // Read the size of the array first
    fread(&arraySize, sizeof(size_t), 1, file);

    // Allocate memory for the array
    struct Index *array = (struct Index*)malloc(arraySize * sizeof(struct Index));

    if (!array) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Read each BookInfo entry from the file
    for (size_t i = 0; i < arraySize; i++) {
        fread(&array[i], sizeof(struct Index), 1, file);
    }

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

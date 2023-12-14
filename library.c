#include "loop.h"
#include "struct.h"
#include "book.h"

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
    struct Index *bookIndexArray = readBookIndexArray(filename,&arraySize);

    size_t deletedSize = 0;
    struct Deleted *bookDeletedArray = readDeletedArray(filename,&deletedSize);
    

    loop(ordering_strategy, filename,&bookIndexArray, &arraySize,&bookDeletedArray, &deletedSize);

    return 0; /** Return success code **/
}

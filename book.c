#include "book.h"
FILE *file;

FILE *openFileWithExtension(const char *filename, const char *extension, const char *mode) {
    // Append the extension to the filename
    char filenameWithExtension[256];  // Adjust the size as needed
    snprintf(filenameWithExtension, sizeof(filenameWithExtension), "%s%s", filename, extension);

    // Open the file with the specified mode
    return fopen(filenameWithExtension, mode);
}

void writeEntryToFile(FILE *file, const struct Entry *entry, struct Index **bookIndexArray, size_t *size) {
    // Calculate the size of the entry excluding the size_t field
    size_t entry_size = sizeof(int) + sizeof(entry->isbn) + strlen(entry->title) + strlen(entry->editorial) + 1; // 1 for the single '|' separator

    // Write the size of the entry to the file
    fwrite(&entry_size, sizeof(size_t), 1, file);

    // Write the key, isbn, title, separator, and editorial to the file
    fwrite(&entry->key, sizeof(int), 1, file);
    fwrite(entry->isbn, sizeof(char), sizeof(entry->isbn), file);
    fwrite(entry->title, sizeof(char), strlen(entry->title), file);
    fwrite("|", sizeof(char), 1, file); // Single '|' separator
    fwrite(entry->editorial, sizeof(char), strlen(entry->editorial), file);

    long int initialOffset = ftell(file) - entry_size -sizeof(size_t);

    struct Index newIndex = {entry->key, initialOffset,entry_size};

    insertIntoIndexArray(bookIndexArray,size,&newIndex);
}

void insertIntoIndexArray(struct Index **array, size_t *size, const struct Index *newBookInfo) {
    size_t i = 0;
    while (i < *size && (*array)[i].key < newBookInfo->key) {
        i++;
    }

    // Shift elements to make space for the new book info
    *array = realloc(*array, (*size + 1) * sizeof(struct Entry));
    memmove(*array + i + 1, *array + i, (*size - i) * sizeof(struct Entry));

    // Insert the new book info at the correct position
    (*array)[i] = *newBookInfo;
    (*size)++;
}

void addBook(const char *command, const char *filename, struct Index **bookIndexArray, size_t *size) {
    // Skip the "add " part
    command += 4;

    // Create a copy of the command for tokenization
    char command_copy[256];  // Adjust the size as needed
    strcpy(command_copy, command);

    // Create an instance of struct Entry
    struct Entry entry;

    // Tokenize and process each value
    char *token = strtok(command_copy, "|");
    entry.key = atoi(token);  // Convert string to integer

    if (binarySearch(*bookIndexArray,*size,entry.key) != -1 ){
        printf("Record with BookID=%d exists\n", entry.key);
    } else {
        // Get the next token (ISBN)
        token = strtok(NULL, "|");
        strncpy(entry.isbn, token, sizeof(entry.isbn));

        // Get the next token (Title) and allocate memory
        token = strtok(NULL, "|");
        entry.title = strdup(token);  // Duplicate the token to dynamically allocate memory

        // Get the next token (Editorial) and allocate memory
        token = strtok(NULL, "|");
        entry.editorial = strdup(token);  // Duplicate the token to dynamically allocate memory

        const char *extension = ".db";
        const char *mode = "ab";

        // Open or create the file with the specified extension and mode
        FILE *file = openFileWithExtension(filename, extension, mode);


        if (file == NULL) {
            fprintf(stderr, "Error opening file %s for writing.\n", filename);
            exit(EXIT_FAILURE);
        }

        // Write the entry to the file
        writeEntryToFile(file, &entry, bookIndexArray, size);

        // Close the file
        fclose(file);

        printf("Record with BookID=%d has been added to the database\n", entry.key);
    }
}

void findBook(const char *command, const char *filename, struct Index *bookIndexArray, size_t size) {
    // Skip the "add " part
    command += 5;

    // Create a copy of the command for tokenization
    char command_copy[20];  // Adjust the size as needed
    strcpy(command_copy, command);

    // Create an instance of struct Entry
    struct Entry entry;

    // Tokenize and process each value
    char *token = strtok(command_copy, "|");
    entry.key = atoi(token);  // Convert string to integer

    int bookIndex = binarySearch(bookIndexArray,size,entry.key);

    if ( bookIndex == -1 ){
        printf("Record with BookID=%d  does not exists\n", entry.key);
    } else {
        const char *extension = ".db";
        const char *mode = "rb";

        // Open or create the file with the specified extension and mode
        FILE *file = openFileWithExtension(filename, extension, mode);
        fseek(file, bookIndexArray[bookIndex].offset, SEEK_SET);

        size_t entry_size;
        size_t read_size = fread(&entry_size, sizeof(size_t), 1, file);

        // Allocate memory to read the entire entry
        char *buffer = (char *)malloc(entry_size);
        if (buffer == NULL) {
            fprintf(stderr, "Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        // Read the entire entry from the file
        read_size = fread(buffer, 1, entry_size, file);

        if (read_size != entry_size) {
            fprintf(stderr, "Error reading entry from file.\n");
            free(buffer);
            exit(EXIT_FAILURE);
        }

        // Interpret the entry
        struct Entry entry;

        // Read key
        memcpy(&entry.key, buffer, sizeof(int));
        buffer += sizeof(int);

        // Read isbn (if needed)
        memcpy(entry.isbn, buffer, sizeof(entry.isbn));
        buffer += sizeof(entry.isbn);

        // Read title (if needed)
        entry.title = strndup(buffer, strcspn(buffer, "|"));
        buffer += strlen(entry.title) + 1; // Move past the title and separator

        // Read editorial (if needed)
        entry.editorial = strdup(buffer);

        printf("%d|%.*s|%s|%s\n", entry.key, (int)sizeof(entry.isbn), entry.isbn, entry.title, entry.editorial);

        // Free the allocated memory for the entry
        free(entry.title);
        fclose(file);
    }

    
}

void read_and_print_data(const char *filename) {
    const char *extension = ".db";
    const char *mode = "ab";

    // Open or create the file with the specified extension and mode
    FILE *file = openFileWithExtension(filename, extension, mode);


    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for reading.\n", filename);
        exit(EXIT_FAILURE);
    }

    while (!feof(file)) {
        size_t entry_size;
        size_t read_size = fread(&entry_size, sizeof(size_t), 1, file);

        if (read_size != 1 || feof(file)) {
            break;
        }

        char *buffer = malloc(entry_size);

        if (buffer == NULL) {
            fprintf(stderr, "Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        read_size = fread(buffer, 1, entry_size, file);

        if (read_size != entry_size) {
            fprintf(stderr, "Error reading entry from file.\n");
            free(buffer);
            exit(EXIT_FAILURE);
        }

       // Interpret the entry and print the information
        struct Entry entry;

        // Read key
        memcpy(&entry.key, buffer, sizeof(int));
        buffer += sizeof(int);

        // Read isbn
        memcpy(entry.isbn, buffer, sizeof(entry.isbn));
        buffer += sizeof(entry.isbn);

        // Read title
        entry.title = strndup(buffer, strcspn(buffer, "|"));
        buffer += strlen(entry.title) + 1; // Move past the title and separator

        // Read editorial
        entry.editorial = strdup(buffer);

        // Print the information
        printf("Key: %d\n", entry.key);
        printf("ISBN: %s\n", entry.isbn);
        printf("Title: %s\n", entry.title);
        printf("Editorial: %s\n", entry.editorial);

        // Free the allocated memory for the entry
        free(entry.title);
    }

    fclose(file);
}

void printIndexData(const char *filename) {
    const char *extension = ".db";
        const char *mode = "rb";

        // Open or create the file with the specified extension and mode
        FILE *file = openFileWithExtension(filename, extension, mode);

    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for reading.\n", filename);
        exit(EXIT_FAILURE);
    }

    size_t entryIndex = 0; // Index counter

    while (!feof(file)) {
        size_t entry_size;
        size_t read_size = fread(&entry_size, sizeof(size_t), 1, file);

        if (read_size != 1 || feof(file)) {
            break;
        }

        // Print the entry index, key, and offset
        printf("Entry #%zu\n", entryIndex);

        // Allocate memory to read the entire entry
        char *buffer = (char *)malloc(entry_size);
        if (buffer == NULL) {
            fprintf(stderr, "Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        // Read the entire entry from the file
        read_size = fread(buffer, 1, entry_size, file);

        if (read_size != entry_size) {
            fprintf(stderr, "Error reading entry from file.\n");
            free(buffer);
            exit(EXIT_FAILURE);
        }

        // Interpret the entry
        struct Entry entry;

        // Read key
        memcpy(&entry.key, buffer, sizeof(int));
        buffer += sizeof(int);

        // Read isbn (if needed)
        memcpy(entry.isbn, buffer, sizeof(entry.isbn));
        buffer += sizeof(entry.isbn);

        // Read title (if needed)
        entry.title = strndup(buffer, strcspn(buffer, "|"));
        buffer += strlen(entry.title) + 1; // Move past the title and separator

        // Read editorial (if needed)
        entry.editorial = strdup(buffer);

        // Print key and offset
        printf("    key: #%d\n", entry.key);
        printf("    offset: #%ld\n", ftell(file) - entry_size -sizeof(size_t)); // Offset is the current file position before reading the entry

        // Free the allocated memory for the entry
        free(entry.title);

        entryIndex++;
    }

    fclose(file);
}

void printRec(const char *filename,const struct Index *array, size_t size) {
    const char *extension = ".db";
    const char *mode = "rb";

    // Open or create the file with the specified extension and mode
    FILE *file = openFileWithExtension(filename, extension, mode);

    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for reading.\n", filename);
        exit(EXIT_FAILURE);
    }

     for (size_t i = 0; i < size; i++) {
        fseek(file, array[i].offset, SEEK_SET);
        size_t entry_size;
        size_t read_size = fread(&entry_size, sizeof(size_t), 1, file);

        if (read_size != 1 || feof(file)) {
            break;
        }

        // Allocate memory to read the entire entry
        char *buffer = (char *)malloc(entry_size);
        if (buffer == NULL) {
            fprintf(stderr, "Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        // Read the entire entry from the file
        read_size = fread(buffer, 1, entry_size, file);

        if (read_size != entry_size) {
            fprintf(stderr, "Error reading entry from file.\n");
            free(buffer);
            exit(EXIT_FAILURE);
        }

        // Interpret the entry
        struct Entry entry;

        // Read key
        memcpy(&entry.key, buffer, sizeof(int));
        buffer += sizeof(int);

        // Read isbn (if needed)
        memcpy(entry.isbn, buffer, sizeof(entry.isbn));
        buffer += sizeof(entry.isbn);

        // Read title (if needed)
        entry.title = strndup(buffer, strcspn(buffer, "|"));
        buffer += strlen(entry.title) + 1; // Move past the title and separator

        // Read editorial (if needed)
        entry.editorial = strdup(buffer);

        printf("%d|%.*s|%s|%s\n", entry.key, (int)sizeof(entry.isbn), entry.isbn, entry.title, entry.editorial);

        // Free the allocated memory for the entry
        free(entry.title);
    }

    fclose(file);
}

void printBookIndexArray(const struct Index *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("Entry #%zu\n",i);
        printf("    key: #%d\n", array[i].key);
        printf("    offset: #%ld\n",array[i].offset);
        printf("    size: #%ld\n",array[i].size);
    }
}

void printDeletedArray(const struct Deleted *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("Entry #%zu\n",i);
        printf("    offset: #%ld\n",array[i].offset);
        printf("    size: #%zu\n",array[i].size);
    }
}

int binarySearch(const struct Index *array, size_t size, int targetId) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (array[mid].key == targetId) {
            return mid;
        } else if (array[mid].key < targetId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // ID not found
    return -1;
}

void insertIntoDeletedArray(struct Deleted **array, size_t *size, const struct Deleted *indexToDelete) {
    size_t i = 0;
    while (i < *size && (*array)[i].size < indexToDelete->size) {
        i++;
    }

    // Shift elements to make space for the new book info
    *array = realloc(*array, (*size + 1) * sizeof(struct Deleted));
    memmove(*array + i + 1, *array + i, (*size - i) * sizeof(struct Deleted));

    // Insert the new book info at the correct position
    (*array)[i] = *indexToDelete;
    (*size)++;
}

void worstInsertIntoDeletedArray(struct Deleted **array, size_t *size, const struct Deleted *indexToDelete) {
    size_t i = 0;
    while (i < *size && (*array)[i].size > indexToDelete->size) {
        i++;
    }

    // Shift elements to make space for the new book info
    *array = realloc(*array, (*size + 1) * sizeof(struct Deleted));
    memmove(*array + i + 1, *array + i, (*size - i) * sizeof(struct Deleted));

    // Insert the new book info at the correct position
    (*array)[i] = *indexToDelete;
    (*size)++;
}

void deleteFromIndexArray(struct Index **array, size_t *size, size_t indexToDelete) {
    if (*size == 0 || indexToDelete >= *size) {
        // Invalid index or empty array, do nothing
        return;
    }

    // Shift elements to fill the gap
    for (size_t i = indexToDelete; i < *size - 1; i++) {
        (*array)[i] = (*array)[i + 1];
    }

    // Reduce the size of the array
    *array = realloc(*array, (*size - 1) * sizeof(struct Index));

    if (*array == NULL && *size > 1) {
        perror("Memory reallocation error");
        exit(EXIT_FAILURE);
    }

    (*size)--;
}

void deleteBook(const char *command,const char *filename,const char *ordering_strategy,struct Index **bookIndexArray,size_t *size,struct Deleted **bookDeletedArray,size_t *deletedSize) {
    // Skip the "del " part
    command += 4;

    // Create a copy of the command for tokenization
    char command_copy[256];  // Adjust the size as needed
    strcpy(command_copy, command);

    // Create an instance of struct Entry
    struct Entry entry;

    char *token = strtok(command_copy, "|");
    entry.key = atoi(token);  // Convert string to integer

    int bookIndex = binarySearch(*bookIndexArray,*size,entry.key);

    if (bookIndex == -1) {
        printf("Record with BookID=%d  does not exists\n", entry.key);
    } else {
        struct Deleted indexToDelete;

        size_t index = bookIndex;
        indexToDelete.offset =  (*bookIndexArray)[index].offset;
        indexToDelete.size =  (*bookIndexArray)[index].size;

        if (strcmp(ordering_strategy, "worst_fit") == 0 ){
            worstInsertIntoDeletedArray(bookDeletedArray,deletedSize, &indexToDelete);
        } else {
            insertIntoDeletedArray(bookDeletedArray,deletedSize, &indexToDelete);
        }

        deleteFromIndexArray(bookIndexArray,size,index);

    }

}
#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

FILE *openFileWithExtension(const char *filename, const char *extension, const char *mode);


// Write an entry to a file and update the book index array
void writeEntryToFile(FILE *file, const struct Entry *entry, struct Index **bookIndexArray, size_t *size);
// Insert a new book info into the book index array
void insertIntoIndexArray(struct Index **array, size_t *size, const struct Index *newBookInfo);
// Add a book entry to the database
void addBook(const char *command, const char *filename, struct Index **bookIndexArray, size_t *size);


// Print the book index array
void printBookIndexArray(const struct Index *array, size_t size);
// Print the index data without using the array
void printIndexData(const char *filename);


// Delete a book entry from the database
void deleteBook(const char *command, const char *filename, const char *ordering_strategy, struct Index **bookIndexArray, size_t *size, struct Deleted **bookDeletedArray, size_t *deletedSize);
// Delete an entry from the book index array
void deleteFromIndexArray(struct Index **array, size_t *size, size_t indexToDelete);
// Insert a deleted entry into the deleted array
void insertIntoDeletedArray(struct Deleted **array, size_t *size, const struct Deleted *indexToDelete);
// Insert a deleted entry into the deleted array using the worst fit strategy
void worstInsertIntoDeletedArray(struct Deleted **array, size_t *size, const struct Deleted *indexToDelete);


// Print the deleted array
void printDeletedArray(const struct Deleted *array, size_t size);


// Find a book entry in the database
void findBook(const char *command, const char *filename, struct Index *bookIndexArray, size_t size);
// Perform binary search on the book index array to find a specific book entry
int binarySearch(const struct Index *array, size_t size, int targetId);


// Print all entries
void printRec(const char *filename,const struct Index *array, size_t size);

#endif /* BOOK_H */

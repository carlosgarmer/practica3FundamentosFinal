#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

FILE *openFileWithExtension(const char *filename, const char *extension, const char *mode);
void writeEntryToFile(FILE *file, const struct Entry *entry, struct Index **bookIndexArray, size_t *size);
void addBook(const char *command, const char *filename, struct Index **bookIndexArray, size_t *size);
void read_and_print_data(const char *filename);
void printIndexData(const char *filename);
void insertIntoIndexArray(struct Index **array, size_t *size, const struct Index *newBookInfo);
void printBookIndexArray(const struct Index *array, size_t size);
void printDeletedArray(const struct Deleted *array, size_t size);
int binarySearch(const struct Index *array, size_t size, int targetId);
void printRec(const char *filename,const struct Index *array, size_t size);
void findBook(const char *command, const char *filename, struct Index *bookIndexArray, size_t size);
void deleteBook(const char *command,const char *filename,const char *ordering_strategy,struct Index **bookIndexArray,size_t *size,struct Deleted **bookDeletedArray,size_t *deletedSize);
void insertIntoDeletedArray(struct Deleted **array, size_t *size, const struct Deleted *newBookInfo);
void worstInsertIntoDeletedArray(struct Deleted **array, size_t *size, const struct Deleted *indexToDelete);
void deleteFromIndexArray(struct Index **array, size_t *size, size_t indexToDelete);

#endif /* BOOK_H */

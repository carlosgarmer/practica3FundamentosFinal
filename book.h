#ifndef BOOK_H
#define BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

FILE *openFileWithExtension(const char *filename, const char *extension, const char *mode);
void write_entry_to_file(FILE *file, const struct Entry *entry, struct Index **bookIndexArray, size_t *size);
void addBook(const char *command, const char *filename, struct Index **bookIndexArray, size_t *size);
void read_and_print_data(const char *filename);
void printIndexData(const char *filename);
void insertIntoArray(struct Index **array, size_t *size, const struct Index *newBookInfo);
void printBookInfoArray(const struct Index *array, size_t size);
int binarySearch(const struct Index *array, size_t size, int targetId);
void printRec(const char *filename,const struct Index *array, size_t size);

#endif /* BOOK_H */

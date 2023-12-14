#ifndef LOOP_H
#define LOOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

void loop(const char *ordering_strategy, const char *filename,struct Index **bookIndexArray, size_t *size,struct Index **bookDeletedArray, size_t *deletedSize);

/**
 * Function to process different commands
 * @param command: Entered command
 * @param ordering_strategy: Strategy for ordering
 * @param filename: Name of the file
 **/
void process_command(const char *command, const char *ordering_strategy, const char *filename, struct Index **bookIndexArray, size_t *size,struct Index **bookDeletedArray, size_t *deletedSize);
void saveBookIndexArray(const char *filename, const struct Index *array, size_t size);
void saveDeletedArray(const char *filename, const char *ordering_strategy, const struct Deleted *array, size_t size);
struct Index* readBookIndexArray(const char *filename, size_t *outSize);
struct Deleted *readDeletedArray(const char *filename, size_t *outSize);

#endif /* LOOP_H */

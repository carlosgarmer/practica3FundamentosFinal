#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>

struct Entry {
    int key;
    char isbn[16];
    char *title;
    char *editorial;
};

struct Index {
    int key;
    long int offset;
    size_t size;
};

#endif

#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>

#define BESTFIT 0
#define WORSTFIT 1
#define FIRSTFIT 2

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

struct Deleted {
    long int offset;
    size_t size;
};

#endif

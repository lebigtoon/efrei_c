#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

typedef struct {
    int *data;
    int size;
    int capacity;
} DynamicArray;

void da_init(DynamicArray *a);

int  da_insert_front(DynamicArray *a, int value);
int  da_insert_back(DynamicArray *a, int value);

int  da_find(const DynamicArray *a, int value);
int  da_get(const DynamicArray *a, int index, int *out);

int  da_remove_front(DynamicArray *a);

void da_free(DynamicArray *a);

#endif

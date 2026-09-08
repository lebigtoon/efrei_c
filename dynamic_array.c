#include "dynamic_array.h"

#include <stdlib.h>
#include <string.h>

#define DA_MIN_CAPACITY 8

void da_init(DynamicArray *a) {
    a->data = NULL;
    a->size = 0;
    a->capacity = 0;
}

static int da_ensure_capacity(DynamicArray *a, int needed) {
    if (needed <= a->capacity) {
        return 0;
    }

    int new_capacity = (a->capacity > 0) ? a->capacity * 2 : DA_MIN_CAPACITY;
    while (new_capacity < needed) {
        new_capacity *= 2;
    }

    int *new_data = realloc(a->data, (size_t)new_capacity * sizeof(int));
    if (new_data == NULL) {
        return -1;
    }
    a->data = new_data;
    a->capacity = new_capacity;
    return 0;
}

int da_insert_back(DynamicArray *a, int value) {
    if (da_ensure_capacity(a, a->size + 1) != 0) {
        return -1;
    }
    a->data[a->size] = value;
    a->size++;
    return 0;
}

int da_insert_front(DynamicArray *a, int value) {
    if (da_ensure_capacity(a, a->size + 1) != 0) {
        return -1;
    }
    memmove(a->data + 1, a->data, (size_t)a->size * sizeof(int));
    a->data[0] = value;
    a->size++;
    return 0;
}

int da_find(const DynamicArray *a, int value) {
    for (int i = 0; i < a->size; i++) {
        if (a->data[i] == value) {
            return i;
        }
    }
    return -1;
}

int da_get(const DynamicArray *a, int index, int *out) {
    if (index < 0 || index >= a->size) {
        return -1;
    }
    *out = a->data[index];
    return 0;
}

int da_remove_front(DynamicArray *a) {
    if (a->size == 0) {
        return -1;
    }
    a->size--;
    memmove(a->data, a->data + 1, (size_t)a->size * sizeof(int));
    return 0;
}

void da_free(DynamicArray *a) {
    free(a->data);
    a->data = NULL;
    a->size = 0;
    a->capacity = 0;
}

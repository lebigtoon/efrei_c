#include "hash_table.h"

#include <stdlib.h>

int hash_good(int value) {
    return value % TABLE_SIZE;
}

int hash_bad(int value) {
    (void)value;
    return 0;
}

void ht_init(HashTable *t) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        t->buckets[i] = NULL;
    }
}

int hash_insert_with(HashTable *t, int value, int (*hash_fn)(int)) {
    int idx = hash_fn(value);

    Entry *entry = malloc(sizeof(Entry));
    if (entry == NULL) {
        return -1;
    }
    entry->value = value;
    entry->next = t->buckets[idx];
    t->buckets[idx] = entry;
    return 0;
}

int hash_contains_with(const HashTable *t, int value, int (*hash_fn)(int)) {
    int idx = hash_fn(value);
    for (const Entry *e = t->buckets[idx]; e != NULL; e = e->next) {
        if (e->value == value) {
            return 1;
        }
    }
    return 0;
}

int hash_insert(HashTable *t, int value) {
    return hash_insert_with(t, value, hash_good);
}

int hash_contains(const HashTable *t, int value) {
    return hash_contains_with(t, value, hash_good);
}

void ht_free(HashTable *t) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *e = t->buckets[i];
        while (e != NULL) {
            Entry *next = e->next;
            free(e);
            e = next;
        }
        t->buckets[i] = NULL;
    }
}

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_SIZE 10007

typedef struct Entry {
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *buckets[TABLE_SIZE];
} HashTable;

int hash_good(int value);
int hash_bad(int value);

void ht_init(HashTable *t);

int hash_insert(HashTable *t, int value);
int hash_contains(const HashTable *t, int value);

int hash_insert_with(HashTable *t, int value, int (*hash_fn)(int));
int hash_contains_with(const HashTable *t, int value, int (*hash_fn)(int));

void ht_free(HashTable *t);

#endif

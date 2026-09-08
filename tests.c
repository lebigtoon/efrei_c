#include "dynamic_array.h"
#include "linked_list.h"
#include "hash_table.h"

#include <assert.h>
#include <stdio.h>

static void test_dynamic_array(void) {
    DynamicArray a;
    da_init(&a);

    for (int i = 0; i < 100; i++) {
        assert(da_insert_back(&a, i) == 0);
    }
    assert(a.size == 100);

    assert(da_insert_front(&a, -1) == 0);
    assert(a.size == 101);

    int out = 0;
    assert(da_get(&a, 0, &out) == 0 && out == -1);
    assert(da_get(&a, 1, &out) == 0 && out == 0);
    assert(da_get(&a, 100, &out) == 0 && out == 99);
    assert(da_get(&a, 101, &out) == -1);
    assert(da_get(&a, -1, &out) == -1);

    assert(da_find(&a, 50) == 51);
    assert(da_find(&a, -1) == 0);
    assert(da_find(&a, 123456) == -1);

    assert(da_remove_front(&a) == 0);
    assert(a.size == 100);
    assert(da_get(&a, 0, &out) == 0 && out == 0);

    while (da_remove_front(&a) == 0) {
    }
    assert(a.size == 0);
    assert(da_remove_front(&a) == -1);

    da_free(&a);
    assert(a.data == NULL && a.size == 0 && a.capacity == 0);
    printf("  dynamic_array : OK\n");
}

static void test_linked_list(void) {
    LinkedList l;
    ll_init(&l);

    for (int i = 0; i < 50; i++) {
        assert(ll_insert_back(&l, i) == 0);
    }
    assert(ll_insert_front(&l, -1) == 0);
    assert(l.size == 51);

    int out = 0;
    assert(ll_get(&l, 0, &out) == 0 && out == -1);
    assert(ll_get(&l, 50, &out) == 0 && out == 49);
    assert(ll_get(&l, 51, &out) == -1);

    assert(ll_find(&l, -1) == 0);
    assert(ll_find(&l, 49) == 50);
    assert(ll_find(&l, 999) == -1);

    assert(ll_remove_front(&l) == 0);
    assert(l.size == 50);
    assert(ll_get(&l, 0, &out) == 0 && out == 0);

    while (ll_remove_front(&l) == 0) {
    }
    assert(l.size == 0);
    assert(ll_remove_front(&l) == -1);

    ll_free(&l);
    assert(l.head == NULL && l.size == 0);
    printf("  linked_list   : OK\n");
}

static void test_hash_table(void) {
    HashTable t;
    ht_init(&t);

    for (int i = 0; i < 2000; i++) {
        assert(hash_insert(&t, i) == 0);
    }
    for (int i = 0; i < 2000; i++) {
        assert(hash_contains(&t, i) == 1);
    }
    assert(hash_contains(&t, 2000) == 0);
    assert(hash_contains(&t, 999999) == 0);

    assert(hash_insert(&t, TABLE_SIZE + 3) == 0);
    assert(hash_insert(&t, 2 * TABLE_SIZE + 3) == 0);
    assert(hash_contains(&t, TABLE_SIZE + 3) == 1);
    assert(hash_contains(&t, 2 * TABLE_SIZE + 3) == 1);

    ht_free(&t);
    for (int i = 0; i < TABLE_SIZE; i++) {
        assert(t.buckets[i] == NULL);
    }

    ht_init(&t);
    for (int i = 0; i < 500; i++) {
        assert(hash_insert_with(&t, i, hash_bad) == 0);
    }
    assert(t.buckets[0] != NULL);
    assert(hash_contains_with(&t, 250, hash_bad) == 1);
    assert(hash_contains_with(&t, 500, hash_bad) == 0);
    ht_free(&t);

    printf("  hash_table    : OK\n");
}

int main(void) {
    printf("Tests de correction :\n");
    test_dynamic_array();
    test_linked_list();
    test_hash_table();
    printf("Tous les tests passent.\n");
    return 0;
}

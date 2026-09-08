#define _POSIX_C_SOURCE 200809L

#include "dynamic_array.h"
#include "linked_list.h"
#include "hash_table.h"

#include <stddef.h>
#include <stdio.h>
#include <time.h>

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static volatile long long g_sink;

static long reps_for(long budget, long n) {
    long r = budget / n;
    return (r < 1) ? 1 : r;
}

static void bench_size(long n) {
    printf("\n========================  n = %ld  ========================\n", n);
    printf("%-26s %16s %16s\n", "operation", "array", "linked list");

    DynamicArray a;
    da_init(&a);
    double t0 = now_sec();
    for (long i = 0; i < n; i++) {
        da_insert_back(&a, (int)i);
    }
    double build_a = now_sec() - t0;

    LinkedList l;
    ll_init(&l);
    t0 = now_sec();
    for (long i = 0; i < n; i++) {
        ll_insert_front(&l, (int)i);
    }
    double build_l = now_sec() - t0;

    printf("%-26s %13.3f ms %13.3f ms\n", "construction (total)",
           build_a * 1e3, build_l * 1e3);

    int target = (int)(n / 2);
    int out = 0;

    long reps = 2000000;
    t0 = now_sec();
    for (long r = 0; r < reps; r++) {
        da_get(&a, target, &out);
        g_sink += out;
    }
    double get_a = (now_sec() - t0) / (double)reps * 1e9;

    reps = reps_for(20000000L, n);
    t0 = now_sec();
    for (long r = 0; r < reps; r++) {
        ll_get(&l, target, &out);
        g_sink += out;
    }
    double get_l = (now_sec() - t0) / (double)reps * 1e9;
    printf("%-26s %13.1f ns %13.1f ns\n", "get(n/2)  [par op]", get_a, get_l);

    reps = reps_for(20000000L, n);
    t0 = now_sec();
    for (long r = 0; r < reps; r++) {
        g_sink += da_find(&a, -1);
    }
    double find_a = (now_sec() - t0) / (double)reps * 1e9;

    t0 = now_sec();
    for (long r = 0; r < reps; r++) {
        g_sink += ll_find(&l, -1);
    }
    double find_l = (now_sec() - t0) / (double)reps * 1e9;
    printf("%-26s %13.1f ns %13.1f ns\n", "find(absent)  [par op]", find_a, find_l);

    reps = reps_for(5000000L, n);
    t0 = now_sec();
    for (long r = 0; r < reps; r++) {
        da_insert_front(&a, 0);
    }
    double if_a = (now_sec() - t0) / (double)reps * 1e9;
    for (long r = 0; r < reps; r++) {
        da_remove_front(&a);
    }

    long reps_l = 1000000;
    t0 = now_sec();
    for (long r = 0; r < reps_l; r++) {
        ll_insert_front(&l, 0);
    }
    double if_l = (now_sec() - t0) / (double)reps_l * 1e9;
    for (long r = 0; r < reps_l; r++) {
        ll_remove_front(&l);
    }
    printf("%-26s %13.1f ns %13.1f ns\n", "insert_front  [par op]", if_a, if_l);

    reps = 1000000;
    t0 = now_sec();
    for (long r = 0; r < reps; r++) {
        da_insert_back(&a, 0);
    }
    double ib_a = (now_sec() - t0) / (double)reps * 1e9;
    a.size -= (int)reps;

    reps_l = reps_for(5000000L, n);
    t0 = now_sec();
    for (long r = 0; r < reps_l; r++) {
        ll_insert_back(&l, 0);
    }
    double ib_l = (now_sec() - t0) / (double)reps_l * 1e9;
    for (long r = 0; r < reps_l; r++) {
        ll_remove_front(&l);
    }
    printf("%-26s %13.1f ns %13.1f ns\n", "insert_back   [par op]", ib_a, ib_l);

    reps = reps_for(50000000L, n);
    t0 = now_sec();
    for (long r = 0; r < reps; r++) {
        long long sum = 0;
        for (int i = 0; i < a.size; i++) {
            sum += a.data[i];
        }
        g_sink += sum;
    }
    double scan_a = (now_sec() - t0) / (double)reps * 1e9;

    reps = reps_for(20000000L, n);
    t0 = now_sec();
    for (long r = 0; r < reps; r++) {
        long long sum = 0;
        for (const Node *c = l.head; c != NULL; c = c->next) {
            sum += c->value;
        }
        g_sink += sum;
    }
    double scan_l = (now_sec() - t0) / (double)reps * 1e9;
    printf("%-26s %13.0f ns %13.0f ns\n", "parcours complet [total]", scan_a, scan_l);

    da_free(&a);
    ll_free(&l);
}

static void bench_hash_run(const char *label, long n, int (*hf)(int)) {
    HashTable t;
    ht_init(&t);

    double t0 = now_sec();
    for (long i = 0; i < n; i++) {
        hash_insert_with(&t, (int)i, hf);
    }
    double build = now_sec() - t0;

    t0 = now_sec();
    long found = 0;
    for (long i = 0; i < n; i++) {
        found += hash_contains_with(&t, (int)i, hf);
    }
    for (long i = 0; i < n; i++) {
        found += hash_contains_with(&t, (int)(n + i), hf);
    }
    double lookup = now_sec() - t0;
    g_sink += found;

    printf("%-12s n=%-8ld  build = %9.3f ms   contains = %10.1f ns/op\n",
           label, n, build * 1e3, lookup / (double)(2 * n) * 1e9);

    ht_free(&t);
}

static void bench_hash(void) {
    printf("\n================  hash : bonne vs mauvaise fonction  ================\n");
    bench_hash_run("hash_good", 20000, hash_good);
    bench_hash_run("hash_good", 200000, hash_good);
    bench_hash_run("hash_bad", 20000, hash_bad);
}

int main(void) {
    const long sizes[] = {1000, 10000, 100000, 1000000};
    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        bench_size(sizes[i]);
    }
    bench_hash();
    printf("\n(sink = %lld)\n", g_sink);
    return 0;
}

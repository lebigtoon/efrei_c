CC     := gcc
CFLAGS := -std=c11 -O2 -Wall -Wextra -Wpedantic
COMMON := dynamic_array.c linked_list.c hash_table.c
HEADERS := dynamic_array.h linked_list.h hash_table.h

.PHONY: all test bench clean

all: tests benchmark

tests: tests.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ tests.c $(COMMON)

benchmark: benchmark.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ benchmark.c $(COMMON)

test: tests
	./tests

bench: benchmark
	./benchmark

clean:
	rm -f tests benchmark

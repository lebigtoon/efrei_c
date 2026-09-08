#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int size;
} LinkedList;

void ll_init(LinkedList *l);

int  ll_insert_front(LinkedList *l, int value);
int  ll_insert_back(LinkedList *l, int value);

int  ll_find(const LinkedList *l, int value);
int  ll_get(const LinkedList *l, int index, int *out);

int  ll_remove_front(LinkedList *l);

void ll_free(LinkedList *l);

#endif

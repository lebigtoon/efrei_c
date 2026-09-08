#include "linked_list.h"

#include <stdlib.h>

void ll_init(LinkedList *l) {
    l->head = NULL;
    l->size = 0;
}

int ll_insert_front(LinkedList *l, int value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return -1;
    }
    node->value = value;
    node->next = l->head;
    l->head = node;
    l->size++;
    return 0;
}

int ll_insert_back(LinkedList *l, int value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return -1;
    }
    node->value = value;
    node->next = NULL;

    if (l->head == NULL) {
        l->head = node;
    } else {
        Node *cur = l->head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = node;
    }
    l->size++;
    return 0;
}

int ll_find(const LinkedList *l, int value) {
    int index = 0;
    for (const Node *cur = l->head; cur != NULL; cur = cur->next) {
        if (cur->value == value) {
            return index;
        }
        index++;
    }
    return -1;
}

int ll_get(const LinkedList *l, int index, int *out) {
    if (index < 0 || index >= l->size) {
        return -1;
    }
    const Node *cur = l->head;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    *out = cur->value;
    return 0;
}

int ll_remove_front(LinkedList *l) {
    if (l->head == NULL) {
        return -1;
    }
    Node *old_head = l->head;
    l->head = old_head->next;
    free(old_head);
    l->size--;
    return 0;
}

void ll_free(LinkedList *l) {
    Node *cur = l->head;
    while (cur != NULL) {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    l->head = NULL;
    l->size = 0;
}

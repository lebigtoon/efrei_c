#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} LinkedList;

int insert_front(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        return -1; 
    }
    new_node->value = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        new_node->next = list->head;
        list->head = new_node;
    }
    return 0;
}

int insert_back(LinkedList* list, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        return -1; 
    }
    new_node->value = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    return 0; 
}

int find(){

}

int remove_front(){

}

int remove_back(){

}
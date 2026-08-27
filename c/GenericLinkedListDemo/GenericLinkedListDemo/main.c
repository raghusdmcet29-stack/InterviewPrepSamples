//
//  main.c
//  GenericLinkedListDemo
//
//  Created by Anussha on 27/08/26.
//
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void* data;
    struct Node* next;
}Node;

typedef struct {
    Node* head;
}LinkedList;

LinkedList* list_create(void){
    LinkedList* list = malloc(sizeof(Node));
    list->head = NULL;
    return list;
}

void list_push_front(LinkedList* list,void* data){
    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->next = list->head;
    list->head = node;
}

void list_print(LinkedList* list, void(*print_fn)(void*)){
    Node* current = list->head;
    while (current != NULL) {
        print_fn(current->data);
        current = current->next;
    }
}

void print_int(void* data){
    printf("%d\n", *(int*)data);
}

void print_string(void* data) {
    printf("%s\n", (char*)data);
}

int main(void){
    LinkedList* int_list = list_create();
    int a = 10, b = 20, c = 30;
    list_push_front(int_list, &a);
    list_push_front(int_list, &b);
    list_push_front(int_list, &c);
    
    printf("Int list:\n");
    list_print(int_list, print_int);
    
    LinkedList* str_list = list_create();
    list_push_front(str_list, "world");
    list_push_front(str_list, "hello");

    printf("String list:\n");
    list_print(str_list, print_string);
    
    return 0;
}

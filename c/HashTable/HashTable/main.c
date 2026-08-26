//
//  main.c
//  HashTable
//
//  Created by Anussha on 26/08/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 8

typedef struct Node{
    char *key;
    int value;
    struct Node *next;
}Node;

typedef struct {
    Node *buckets[TABLE_SIZE];
}HashTable;

void initTable(HashTable *table){
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
}

void destroyTable(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = table->buckets[i];
        while (current != NULL) {
            Node *toDelete = current;
            current = current->next;   // advance BEFORE freeing toDelete
            free(toDelete->key);       // free the strdup'd string first
            free(toDelete);            // then free the node itself
        }
        table->buckets[i] = NULL;      // avoid dangling pointer
    }
    printf("Table destroyed\n");
}

unsigned int hash(const char *key){
    unsigned int h = 0;
    while (*key) {
        h = h * 31 + (unsigned char)(*key);
        key++;
    }
    return h % TABLE_SIZE;
}

void insert(HashTable *table,const char *key,int value){
    unsigned int index = hash(key);
    Node *newNode = malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = table->buckets[index];  // insert at head of chain
    table->buckets[index] = newNode;
    printf("Inserted key=%s, value=%d at bucket %u\n", key, value, index);
}

int get(HashTable *table, const char *key,int *outValue){
    unsigned int index = hash(key);
    Node *current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            *outValue = current->value;
            return 1; //found
        }
        current = current->next;
    }
    return 0;  //not found
}

void printTable(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Bucket %d: ", i);
        Node *current = table->buckets[i];
        while (current != NULL) {
            printf("(%s:%d) -> ", current->key, current->value);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int main(void) {
    HashTable table;
    initTable(&table);

    insert(&table, "apple", 10);
    insert(&table, "banana", 20);
    insert(&table, "cherry", 30);
    insert(&table, "cherry1", 40);
    insert(&table, "cherry2", 50);
    insert(&table, "cherry3", 60);
    insert(&table, "cherry4", 70);
    insert(&table, "cherry5", 80);
    insert(&table, "cherry6", 90);
    insert(&table, "cherry7", 95);

    int value;
    if (get(&table, "banana", &value)) {
        printf("Found banana = %d\n", value);
    } else {
        printf("banana not found\n");
    }

    if (get(&table, "grape", &value)) {
        printf("Found grape = %d\n", value);
    } else {
        printf("grape not found\n");
    }
    printTable(&table);
    destroyTable(&table);
    return 0;
}

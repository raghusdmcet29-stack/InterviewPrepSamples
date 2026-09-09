//
//  main.c
//  CompositePattern
//
//  Created by Anussha on 09/09/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NODE_FILE, NODE_FOLDER } NodeType;

typedef struct FileSystemNode {
    NodeType type;
    char name[64];
    union {
        struct {
            int size;
        } file;
        struct {
            struct FileSystemNode** children;
            int childCount;
            int capacity;
        } folder;
    } data;
} FileSystemNode;

FileSystemNode* createFile(const char* name, int size) {
    FileSystemNode* node = malloc(sizeof(FileSystemNode));
    node->type = NODE_FILE;
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->data.file.size = size;
    return node;
}

int getSize(FileSystemNode* node) {
    if (node->type == NODE_FILE) {
        return node->data.file.size;
    }
    int total = 0;
    for (int i = 0; i < node->data.folder.childCount; i++) {
        total += getSize(node->data.folder.children[i]);
    }
    return total;
}

void freeNode(FileSystemNode* node) {
    if (node->type == NODE_FOLDER) {
        for (int i = 0; i < node->data.folder.childCount; i++) {
            freeNode(node->data.folder.children[i]);
        }
        free(node->data.folder.children);
    }
    free(node);
}

FileSystemNode* createFolder(const char* name) {
    FileSystemNode* node = malloc(sizeof(FileSystemNode));
    node->type = NODE_FOLDER;
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->data.folder.childCount = 0;
    node->data.folder.capacity = 4;
    node->data.folder.children = malloc(sizeof(FileSystemNode*) * node->data.folder.capacity);
    return node;
}

void addChild(FileSystemNode* folder, FileSystemNode* child) {
    if (folder->data.folder.childCount == folder->data.folder.capacity) {
        folder->data.folder.capacity *= 2;
        folder->data.folder.children = realloc(
            folder->data.folder.children,
            sizeof(FileSystemNode*) * folder->data.folder.capacity
        );
    }
    folder->data.folder.children[folder->data.folder.childCount++] = child;
}

int main(void) {
    FileSystemNode* documents = createFolder("Documents");
    addChild(documents, createFile("resume.pdf", 120));
    addChild(documents, createFile("photo.jpg", 300));

    FileSystemNode* archive = createFolder("Archive");
    addChild(archive, createFile("old_report.docx", 80));

    addChild(documents, archive);

    printf("%s total size: %d KB\n", documents->name, getSize(documents));

    freeNode(documents); // recursively frees everything, including archive and its file
    return 0;
}


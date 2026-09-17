//
//  main.c
//  FlyweightPattern
//
//  Created by Anussha on 17/09/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once
#define MAX_TREE_TYPES 16
#define MAX_TREES 128
typedef struct {
    char species[32];
    char texture[64];
    char color[32];
} TreeType;

typedef struct {
    TreeType* types[MAX_TREE_TYPES];
    char names[MAX_TREE_TYPES][32];
    int count;
} TreeFactory;

void treeFactory_init(TreeFactory* factory) {
    factory->count = 0;
}

TreeType* treeFactory_getTreeType(TreeFactory* factory, const char* species, const char* texture, const char* color) {
    for (int i = 0; i < factory->count; i++) {
        if (strcmp(factory->names[i], species) == 0) {
            return factory->types[i];
        }
    }

    TreeType* newType = malloc(sizeof(TreeType));
    strncpy(newType->species, species, sizeof(newType->species) - 1);
    newType->species[sizeof(newType->species) - 1] = '\0';
    strncpy(newType->texture, texture, sizeof(newType->texture) - 1);
    newType->texture[sizeof(newType->texture) - 1] = '\0';
    strncpy(newType->color, color, sizeof(newType->color) - 1);
    newType->color[sizeof(newType->color) - 1] = '\0';

    printf("Created new TreeType: %s\n", newType->species);

    factory->types[factory->count] = newType;
    strncpy(factory->names[factory->count], species, sizeof(factory->names[0]) - 1);
    factory->names[factory->count][sizeof(factory->names[0]) - 1] = '\0';
    factory->count++;

    return newType;
}

void treeFactory_destroy(TreeFactory* factory) {
    for (int i = 0; i < factory->count; i++) {
        free(factory->types[i]);
    }
}
typedef struct {
    int x, y;
    TreeType* type;   // borrowed pointer — caller (Forest) owns the real memory via the factory
} Tree;

void tree_render(const Tree* tree) {
    printf("Rendering %s tree at (%d, %d) with texture %s\n",
           tree->type->species, tree->x, tree->y, tree->type->texture);
}

typedef struct {
    Tree trees[MAX_TREES];
    int count;
    TreeFactory factory;
} Forest;

void forest_init(Forest* forest) {
    forest->count = 0;
    treeFactory_init(&forest->factory);
}

void forest_plantTree(Forest* forest, int x, int y, const char* species, const char* texture, const char* color) {
    TreeType* type = treeFactory_getTreeType(&forest->factory, species, texture, color);
    forest->trees[forest->count].x = x;
    forest->trees[forest->count].y = y;
    forest->trees[forest->count].type = type;
    forest->count++;
}

void forest_render(const Forest* forest) {
    for (int i = 0; i < forest->count; i++) {
        tree_render(&forest->trees[i]);
    }
}

void forest_destroy(Forest* forest) {
    treeFactory_destroy(&forest->factory);
}

int main(void) {
    Forest forest;
    forest_init(&forest);

    forest_plantTree(&forest, 1, 2, "Oak", "oak.png", "green");
    forest_plantTree(&forest, 5, 8, "Pine", "pine.png", "darkgreen");
    forest_plantTree(&forest, 3, 3, "Oak", "oak.png", "green");
    forest_plantTree(&forest, 9, 1, "Oak", "oak.png", "green");
    forest_plantTree(&forest, 7, 6, "Pine", "pine.png", "darkgreen");

    forest_render(&forest);

    forest_destroy(&forest);
    return 0;
}

/*int main(void) {
    TreeFactory factory;
    treeFactory_init(&factory);

    TreeType* oak1 = treeFactory_getTreeType(&factory, "Oak", "oak.png", "green");
    TreeType* pine1 = treeFactory_getTreeType(&factory, "Pine", "pine.png", "darkgreen");
    TreeType* oak2 = treeFactory_getTreeType(&factory, "Oak", "oak.png", "green");
    TreeType* oak3 = treeFactory_getTreeType(&factory, "Oak", "oak.png", "green");
    TreeType* pine2 = treeFactory_getTreeType(&factory, "Pine", "pine.png", "darkgreen");

    printf("oak1 == oak2: %d\n", oak1 == oak2);
    printf("oak1 == oak3: %d\n", oak1 == oak3);
    printf("oak1 == pine1: %d\n", oak1 == pine1);

    treeFactory_destroy(&factory);
    return 0;
}*/





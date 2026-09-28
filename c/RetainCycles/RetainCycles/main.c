//
//  main.c
//  RetainCycles
//
//  Created by Anussha on 28/09/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Non Retain cycle code


typedef struct Apartment Apartment;

typedef struct Person {
    char name[32];
    int refCount;
    Apartment *apartment;
} Person;

struct Apartment {
    char unit[32];
    int refCount;
    Person *tenant;
};

void personRelease(Person *p);
void apartmentRelease(Apartment *a);

Person *personCreate(const char *name) {
    Person *p = malloc(sizeof(Person));
    strcpy(p->name, name);
    p->refCount = 1;
    p->apartment = NULL;
    printf("Person %s created\n", p->name);
    return p;
}

Apartment *apartmentCreate(const char *unit) {
    Apartment *a = malloc(sizeof(Apartment));
    strcpy(a->unit, unit);
    a->refCount = 1;
    a->tenant = NULL;
    printf("Apartment %s created\n", a->unit);
    return a;
}

void personRelease(Person *p) {
    p->refCount--;
    if (p->refCount == 0) {
        printf("Person %s freed\n", p->name);
        if (p->apartment) apartmentRelease(p->apartment);
        free(p);
    }
}

void apartmentRelease(Apartment *a) {
    a->refCount--;
    if (a->refCount == 0) {
        printf("Apartment %s freed\n", a->unit);
        // EDIT 2: the line that released the tenant is removed
        free(a);
    }
}

int main(void) {
    Person *alice = personCreate("Alice");
    Apartment *flat = apartmentCreate("4B");

    alice->apartment = flat;
    flat->refCount++;

    flat->tenant = alice;
    // EDIT 1: alice->refCount++ is removed (weak, non-owning link)

    personRelease(alice);
    apartmentRelease(flat);
    
  /*
    // below code cause dangling pointer issue
    Person *carol = personCreate("Carol");
    Apartment *flat2 = apartmentCreate("7C");
    flat2->tenant = carol;

    personRelease(carol);
    printf("About to read the tenant\n");
    printf("Tenant is %s\n", flat2->tenant->name);

    apartmentRelease(flat2);
*/
    printf("End of program\n");
    
    return 0;
}


// coded which ahs retain cycle
/*typedef struct Apartment Apartment;

typedef struct Person {
    char name[32];
    int refCount;
    Apartment *apartment;
} Person;

struct Apartment {
    char unit[32];
    int refCount;
    Person *tenant;
};

void personRelease(Person *p);
void apartmentRelease(Apartment *a);

Person *personCreate(const char *name) {
    Person *p = malloc(sizeof(Person));
    strcpy(p->name, name);
    p->refCount = 1;
    p->apartment = NULL;
    printf("Person %s created\n", p->name);
    return p;
}

Apartment *apartmentCreate(const char *unit) {
    Apartment *a = malloc(sizeof(Apartment));
    strcpy(a->unit, unit);
    a->refCount = 1;
    a->tenant = NULL;
    printf("Apartment %s created\n", a->unit);
    return a;
}

void personRelease(Person *p) {
    p->refCount--;
    if (p->refCount == 0) {
        printf("Person %s freed\n", p->name);
        if (p->apartment) apartmentRelease(p->apartment);
        free(p);
    }
}

void apartmentRelease(Apartment *a) {
    a->refCount--;
    if (a->refCount == 0) {
        printf("Apartment %s freed\n", a->unit);
        if (a->tenant) personRelease(a->tenant);
        free(a);
    }
}

int main(void) {
    Person *alice = personCreate("Alice");
    Apartment *flat = apartmentCreate("4B");

    alice->apartment = flat;
    flat->refCount++;

    flat->tenant = alice;
    alice->refCount++;

    personRelease(alice);
    apartmentRelease(flat);

    printf("End of program\n");
    return 0;
}
*/

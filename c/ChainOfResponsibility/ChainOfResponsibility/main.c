//
//  main.c
//  ChainOfResponsibility
//
//  Created by Anussha on 10/09/26.
//

#include <stdio.h>

typedef enum { LOW, MEDIUM, HIGH } Priority;

typedef struct {
    const char* description;
    Priority priority;
} Ticket;

typedef struct SupportHandler {
    void (*handle)(struct SupportHandler* self, Ticket ticket);
    struct SupportHandler* next;
} SupportHandler;


void level1Handle(SupportHandler* self, Ticket ticket) {
    if (ticket.priority == LOW) {
        printf("Level1Support resolved: %s\n", ticket.description);
    } else {
        printf("Level1Support cannot resolve, escalating...\n");
        if (self->next) self->next->handle(self->next, ticket);
    }
}

void level2Handle(SupportHandler* self, Ticket ticket) {
    if (ticket.priority == MEDIUM) {
        printf("Level2Support resolved: %s\n", ticket.description);
    } else {
        printf("Level2Support cannot resolve, escalating...\n");
        if (self->next) self->next->handle(self->next, ticket);
    }
}

void managerHandle(SupportHandler* self, Ticket ticket) {
    if (ticket.priority == HIGH) {
        printf("Manager resolved: %s\n", ticket.description);
    } else {
        printf("Manager cannot resolve, and no one left to escalate to.\n");
    }
}

int main(void){
    SupportHandler level1 = { level1Handle, NULL };
    SupportHandler level2 = { level2Handle, NULL };
    SupportHandler manager = { managerHandle, NULL };
    
    level1.next = &level2;
    level2.next = &manager;
    
    Ticket tickets[] = {
            {"Forgot password", LOW},
            {"App crashing intermittently", MEDIUM},
            {"Data breach reported", HIGH}
        };

    int count = sizeof(tickets) / sizeof(tickets[0]);

    for (int i = 0; i < count; i++) {
        level1.handle(&level1, tickets[i]);
    }

    return 0;
}

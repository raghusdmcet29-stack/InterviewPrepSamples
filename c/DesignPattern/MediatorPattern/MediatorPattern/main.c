//
//  main.c
//  MediatorPattern
//
//  Created by Anussha on 17/09/26.
//

#include <stdio.h>
#include <string.h>
#pragma once

#define MAX_NAME 32
#define MAX_USERS 16
typedef struct ChatRoomMediator ChatRoomMediator;  // forward declaration
typedef struct {
    char name[MAX_NAME];
    ChatRoomMediator* mediator;   // borrowed pointer
} User;

void mediator_init(ChatRoomMediator* mediator);
void mediator_registerUser(ChatRoomMediator* mediator, User* user);
void mediator_send(ChatRoomMediator* mediator, const char* message, User* sender);



struct ChatRoomMediator {
    User* users[MAX_USERS];   // borrowed pointers — mediator does NOT own these
    int count;
};

void user_init(User* user, const char* name, ChatRoomMediator* mediator) {
    strncpy(user->name, name, MAX_NAME - 1);
    user->name[MAX_NAME - 1] = '\0';
    user->mediator = mediator;
}

void user_send(User* user, const char* message) {
    mediator_send(user->mediator, message, user);
}

void user_receive(User* user, const char* message, const char* senderName) {
    printf("%s received: %s: %s\n", user->name, senderName, message);
}

void mediator_init(ChatRoomMediator* mediator) {
    mediator->count = 0;
}

void mediator_registerUser(ChatRoomMediator* mediator, User* user) {
    mediator->users[mediator->count] = user;
    mediator->count++;
}

void mediator_send(ChatRoomMediator* mediator, const char* message, User* sender) {
    for (int i = 0; i < mediator->count; i++) {
        if (mediator->users[i] != sender) {
            user_receive(mediator->users[i], message, sender->name);
        }
    }
}

int main(void) {
    ChatRoomMediator mediator;
    mediator_init(&mediator);

    User alice, bob, charlie;
    user_init(&alice, "Alice", &mediator);
    user_init(&bob, "Bob", &mediator);
    user_init(&charlie, "Charlie", &mediator);

    mediator_registerUser(&mediator, &alice);
    mediator_registerUser(&mediator, &bob);
    mediator_registerUser(&mediator, &charlie);

    user_send(&alice, "Hello everyone");

    return 0;
}



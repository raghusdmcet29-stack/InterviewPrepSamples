//
//  main.cpp
//  MediatorPattern
//
//  Created by Anussha on 17/09/26.
//

// ChatMediator.h
#pragma once
#include <string>
#include <iostream>
#include <vector>

class User;  // forward declaration

class ChatMediator {
public:
    virtual ~ChatMediator() = default;
    virtual void send(const std::string& message, User* sender) = 0;
    virtual void registerUser(User* user) = 0;
};


class User {
private:
    std::string name;
    ChatMediator* mediator;   // non-owning, borrowed pointer

public:
    User(std::string name, ChatMediator* mediator)
        : name(std::move(name)), mediator(mediator) {}

    const std::string& getName() const { return name; }

    void send(const std::string& message) {
        mediator->send(message, this);
    }

    void receive(const std::string& message, const std::string& senderName) {
        std::cout << name << " received: " << senderName << ": " << message << "\n";
    }
};

class ChatRoomMediator : public ChatMediator {
private:
    std::vector<User*> users;   // non-owning — mediator does NOT own the users

public:
    void registerUser(User* user) override {
        users.push_back(user);
    }

    void send(const std::string& message, User* sender) override {
        for (User* user : users) {
            if (user != sender) {
                user->receive(message, sender->getName());
            }
        }
    }
};

int main() {
    ChatRoomMediator mediator;

    User alice("Alice", &mediator);
    User bob("Bob", &mediator);
    User charlie("Charlie", &mediator);

    mediator.registerUser(&alice);
    mediator.registerUser(&bob);
    mediator.registerUser(&charlie);

    alice.send("Hello everyone");

    return 0;
}

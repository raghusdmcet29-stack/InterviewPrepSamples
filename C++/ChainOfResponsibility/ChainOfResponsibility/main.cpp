//
//  main.cpp
//  ChainOfResponsibility
//
//  Created by Anussha on 10/09/26.
//

#include <iostream>

enum class Priority { Low, Medium, High };

struct Ticket{
    std::string description;
    Priority priority;
};

class SupportHandler {
public:
    virtual ~SupportHandler() = default;
    virtual void handle(const Ticket& ticket) = 0;
    
    void setNext(SupportHandler* nextHandler){
        next = nextHandler;
    }
protected:
    SupportHandler* next = nullptr;
};

class Level1Support : public SupportHandler {
public:
    void handle(const Ticket& ticket) override {
        if (ticket.priority == Priority::Low) {
            std::cout << "Level1Support resolved: " << ticket.description << "\n";
        } else {
            std::cout << "Level1Support cannot resolve, escalating...\n";
            if (next) next->handle(ticket);
        }
    }
};

class Level2Support : public SupportHandler {
public:
    void handle(const Ticket& ticket) override {
        if (ticket.priority == Priority::Medium) {
            std::cout << "Level2Support resolved: " << ticket.description << "\n";
        } else {
            std::cout << "Level2Support cannot resolve, escalating...\n";
            if (next) next->handle(ticket);
        }
    }
};

class Manager : public SupportHandler {
public:
    void handle(const Ticket& ticket) override {
        if (ticket.priority == Priority::High) {
            std::cout << "Manager resolved: " << ticket.description << "\n";
        } else {
            std::cout << "Manager cannot resolve, and no one left to escalate to.\n";
        }
    }
};

int main(){
    Level1Support level1;
    Level2Support level2;
    Manager manager;
    
    level1.setNext(&level2);
    level2.setNext(&manager);
    
    std::vector<Ticket> tickets = {
            {"Forgot password", Priority::Low},
            {"App crashing intermittently", Priority::Medium},
            {"Data breach reported", Priority::High}
        };
    
    for (const auto& ticket : tickets) {
            level1.handle(ticket);
        }

        return 0;
}

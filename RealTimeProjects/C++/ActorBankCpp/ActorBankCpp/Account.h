//
//  Account.h
//  ActorBankCpp
//
//  Created by Anussha on 23/10/26.
//

#ifndef Account_h
#define Account_h

#pragma once
#include <mutex>
#include <string>
#include <utility>

class Account {
    std::string id;
    int balance;
    bool frozen = false;
    std::mutex m;
public:
    Account(std::string id = "", int balance = 0)
        : id(std::move(id)), balance(balance) {}

    bool deposit(int amount) {
        std::lock_guard<std::mutex> lock(m);
        if (frozen) return false;
        balance += amount;
        return true;
    }

    bool withdraw(int amount) {
        std::lock_guard<std::mutex> lock(m);
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }

    void freeze() {
        std::lock_guard<std::mutex> lock(m);
        frozen = true;
    }

    bool isFrozen() {
        std::lock_guard<std::mutex> lock(m);
        return frozen;
    }
    int getBalance() {
        std::lock_guard<std::mutex> lock(m);
        return balance;
    }
};

#endif /* Account_h */

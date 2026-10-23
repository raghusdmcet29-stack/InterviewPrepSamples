//
//  Bank.h
//  ActorBankCpp
//
//  Created by Anussha on 23/10/26.
//

#ifndef Bank_h
#define Bank_h

#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include "Account.h"
#include "Journal.h"

class Bank {
    std::map<std::string, std::unique_ptr<Account>> accounts;
    std::mutex m;
    int nextSequence = 1;
    std::string journalPath;
    void openInMemory(const std::string& id, int balance) {
            accounts[id] = std::make_unique<Account>(id, balance);
        }
public:
    void loadFromJournal() {
        auto balances = rebuildBalances(journalPath);
        for (const auto& [id, balance] : balances) {
            openInMemory(id, balance);
        }
    }
    
    Bank(const std::string& path) : journalPath(path), nextSequence(nextSequenceNumber(path)) {}
    void openAccount(const std::string& id, int balance) {
            std::lock_guard<std::mutex> lock(m);
            appendLine(journalPath, openingLine(id, balance));
            openInMemory(id, balance);
        }
    

    std::optional<int> balance(const std::string& id) {
        std::lock_guard<std::mutex> lock(m);
        auto it = accounts.find(id);
        if (it == accounts.end()) return std::nullopt;
        return it->second->getBalance();
    }
    bool deposit(const std::string& id, int amount) {
        std::lock_guard<std::mutex> lock(m);
        auto it = accounts.find(id);
        if (it == accounts.end()) return false;
        if (!appendLine(journalPath, journalLine(nextSequence, id, amount))) return false;
        nextSequence++;
        return it->second->deposit(amount);
    }
    
    bool transfer(const std::string& fromId, const std::string& toId, int amount) {
        std::lock_guard<std::mutex> lock(m);
        auto fromIt = accounts.find(fromId);
        auto toIt = accounts.find(toId);
        if (fromIt == accounts.end() || toIt == accounts.end()) return false;
        Account& from = *fromIt->second;
        Account& to = *toIt->second;
        if (to.isFrozen() || from.getBalance() < amount) return false;
        from.withdraw(amount);
        to.deposit(amount);
        return true;
    }
    void freeze(const std::string& id) {
        std::lock_guard<std::mutex> lock(m);
        auto it = accounts.find(id);
        if (it != accounts.end()) it->second->freeze();
    }
};

#endif /* Bank_h */

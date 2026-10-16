//
//  Summary.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Summary_h
#define Summary_h

#pragma once
#include <future>
#include <map>
#include <utility>
#include <vector>
#include "Bank.hpp"

struct Summary {
    std::map<int, long long> balances;
    long long total = 0;
};

// One small task: read one account's balance
std::pair<int, long long> readBalance(Bank& bank, int id) {
    return {id, bank.balanceOf(id)};
}

// Start one task per account, then collect the results
inline Summary summarize(Bank& bank, const std::vector<int>& ids) {
    std::vector<std::future<std::pair<int, long long>>> futures;
    for (int id : ids) {
        futures.push_back(std::async(std::launch::async, readBalance, std::ref(bank), id));
    }

    Summary s;
    for (auto& f : futures) {
        auto [id, balance] = f.get();   // waits for this task to finish
        s.balances[id] = balance;
        s.total += balance;
    }
    return s;
}

#endif /* Summary_h */

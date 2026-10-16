//
//  Account.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Account_h
#define Account_h

#pragma once
#include <mutex>

struct Account {
    int id;
    long long balance;
    std::mutex lock;   // only one thread changes this account at a time

    Account(int id_, long long balance_) : id(id_), balance(balance_) {}
};


#endif /* Account_h */

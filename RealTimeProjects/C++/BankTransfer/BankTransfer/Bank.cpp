//
//  Bank.cpp
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#include "Bank.hpp"

void Bank::open(int id, long long balance){
    accounts_[id] = std::make_unique<Account>(id,balance);
}

long long Bank::balanceOf(int id){
    Account* a = accounts_.at(id).get();
    std::lock_guard<std::mutex> guard(a->lock);
    return a->balance;
}

bool Bank::transfer(const TransferRequest& r) {
    auto fromIt = accounts_.find(r.from);
    auto toIt = accounts_.find(r.to);
    if (fromIt == accounts_.end() || toIt == accounts_.end()) {
        failed_++;
        return false;
    }
    Account* from = fromIt->second.get();
    Account* to = toIt->second.get();
    
    // Stage 3: always lock the lower-numbered account first
    Account* first = from->id < to->id ? from : to;
    Account* second = from->id < to->id ? to : from;
    
    std::lock_guard<std::mutex> g1(first->lock);
    std::lock_guard<std::mutex> g2(second->lock);
    
    if (from->balance < r.amount) {
        failed_++;
        return false;
    }
    from->balance -= r.amount;
    to->balance += r.amount;
    success_++;
    return true;
}


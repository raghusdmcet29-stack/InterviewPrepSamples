//
//  Bank.hpp
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Bank_hpp
#define Bank_hpp

#include <stdio.h>
#pragma once
#include <atomic>
#include <map>
#include <memory>
#include "Account.h"
#include "TransferRequest.h"

class Bank{
public:
    void open(int id, long long balance);
    long long balanceOf(int id);
    bool transfer(const TransferRequest& r);
    
    long long successCount() const {return success_.load();}
    long long failedCount() const { return failed_.load(); }
private:
    std::map<int, std::unique_ptr<Account>> accounts_;
    std::atomic<long long> success_{0};
    std::atomic<long long> failed_{0};
};

#endif /* Bank_hpp */

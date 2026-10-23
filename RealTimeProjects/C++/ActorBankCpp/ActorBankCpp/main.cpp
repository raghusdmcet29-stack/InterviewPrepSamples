//
//  main.cpp
//  ActorBankCpp
//
//  Created by Anussha on 23/10/26.
//

#include <cstdio>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include "Account.h"
#include "Bank.h"
#include "Journal.h"

int main() {
    std::cout << std::boolalpha;

    // Bank: open, balance, deposit
    std::string testBankPath = "/Users/anussha/ActorBankData/journal-cpp-test-bank.txt";
    std::remove(testBankPath.c_str());
    Bank bank(testBankPath);
    bank.openAccount("A", 100);
    std::cout << bank.balance("A").value_or(-1) << "\n";   // expect 100
    std::cout << bank.balance("Z").value_or(-1) << "\n";   // expect -1
    std::cout << bank.deposit("A", 20) << "\n";            // expect true
    std::cout << bank.balance("A").value_or(-1) << "\n";   // expect 120
    std::cout << bank.deposit("Z", 5) << "\n";             // expect false

    // Bank: transfers
    bank.openAccount("B", 50);
    std::cout << bank.transfer("A", "B", 30) << "\n";      // expect true
    std::cout << bank.balance("A").value_or(-1) << "\n";   // expect 90
    std::cout << bank.balance("B").value_or(-1) << "\n";   // expect 80
    std::cout << bank.transfer("A", "Z", 10) << "\n";      // expect false
    std::cout << bank.transfer("B", "A", 1000) << "\n";    // expect false

    // Bank: freeze
    bank.freeze("B");
    std::cout << bank.deposit("B", 10) << "\n";            // expect false
    std::cout << bank.balance("B").value_or(-1) << "\n";   // expect 80
    std::cout << bank.transfer("A", "B", 5) << "\n";       // expect false
    std::cout << bank.transfer("B", "A", 5) << "\n";       // expect true

    // Journal: format and append
    std::cout << journalLine(4, "A", -30) << "\n";         // expect 4,A,-30
    std::cout << journalLine(5, "B", 100) << "\n";         // expect 5,B,100
    std::string testPath = "/Users/anussha/ActorBankData/journal-cpp.txt";
    std::cout << appendLine(testPath, journalLine(6, "A", 15)) << "\n";  // expect true

    // Journal: read and split
    for (const auto& l : readLines(testPath)) {
        std::cout << "read: " << l << "\n";
    }
    for (const auto& f : splitFields("6,A,15")) {
        std::cout << "field: " << f << "\n";
    }

    // Journal: number and entry parsing
    int n = 0;
    std::cout << parseNumber("15", n) << " " << n << "\n";      // expect true 15
    std::cout << parseNumber("abc", n) << "\n";                  // expect false

    JournalEntry e{};
    std::cout << parseEntry("6,A,15", e) << " " << e.sequence << " " << e.accountId << " " << e.amount << "\n";  // expect true 6 A 15
    std::cout << parseEntry("6,A", e) << "\n";     // expect false
    std::cout << parseEntry("x,A,15", e) << "\n";  // expect false

    // Journal: rebuild balances
    std::string rebuildPath = "/Users/anussha/ActorBankData/journal-rebuild.txt";
    std::remove(rebuildPath.c_str());
    appendLine(rebuildPath, journalLine(6, "A", 15));
    appendLine(rebuildPath, journalLine(7, "A", -5));
    appendLine(rebuildPath, journalLine(8, "B", 20));
    auto rb = rebuildBalances(rebuildPath);
    std::cout << rb["A"] << " " << rb["B"] << "\n";  // expect 10 20

    // Bank: journal write, then restart
    std::string bankPath = "/Users/anussha/ActorBankData/journal-cpp-bank.txt";
    std::remove(bankPath.c_str());
    Bank jb(bankPath);
    jb.openAccount("A", 100);
    jb.deposit("A", 20);
    jb.deposit("A", 5);
    std::cout << jb.balance("A").value_or(-1) << "\n";  // expect 125

    // Simulate a restart: a new Bank reads the journal and continues the sequence
    Bank jb2(bankPath);
    jb2.loadFromJournal();
    jb2.deposit("A", 1);  // written as sequence 3
    std::cout << jb2.balance("A").value_or(-1) << "\n";  // expect 126
    
    Bank jb3(bankPath);
    jb3.loadFromJournal();
    std::cout << jb3.balance("A").value_or(-1) << "\n";  // expect 126
}

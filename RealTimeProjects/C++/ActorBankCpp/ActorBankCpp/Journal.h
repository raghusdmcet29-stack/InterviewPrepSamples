//
//  Journal.h
//  ActorBankCpp
//
//  Created by Anussha on 23/10/26.
//

#ifndef Journal_h
#define Journal_h


#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdio>
#include <vector>
#include <stdexcept>

inline std::string journalLine(int sequence, const std::string& accountId, int amount) {
    return std::to_string(sequence) + "," + accountId + "," + std::to_string(amount);
}

inline std::string openingLine(const std::string& accountId, int balance) {
    return "open," + accountId + "," + std::to_string(balance);
}

inline bool appendLine(const std::string& path, const std::string& line) {
    std::ofstream out(path, std::ios::app);
    if (!out) return false;
    out << line << "\n";
    return true;
}

inline std::vector<std::string> readLines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }
    return lines;
}

inline std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

inline bool parseNumber(const std::string& text, int& out) {
    try {
        out = std::stoi(text);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

struct JournalEntry {
    int sequence;
    std::string accountId;
    int amount;
};

inline bool parseEntry(const std::string& line, JournalEntry& out) {
    auto fields = splitFields(line);
    if (fields.size() != 3) return false;
    int seq = 0, amt = 0;
    if (!parseNumber(fields[0], seq)) return false;
    if (!parseNumber(fields[2], amt)) return false;
    out = JournalEntry{seq, fields[1], amt};
    return true;
}

inline bool parseOpening(const std::string& line, std::string& id, int& balance) {
    auto fields = splitFields(line);
    if (fields.size() != 3 || fields[0] != "open") return false;
    int bal = 0;
    if (!parseNumber(fields[2], bal) || bal < 0) return false;
    id = fields[1];
    balance = bal;
    return true;
}

inline std::map<std::string, int> rebuildBalances(const std::string& path) {
    std::map<std::string, int> balances;
    for (const auto& line : readLines(path)) {
        std::string id;
        int bal = 0;
        if (parseOpening(line, id, bal)) {
            balances[id] = bal;
            continue;
        }
        JournalEntry e{};
        if (parseEntry(line, e)) {
            balances[e.accountId] += e.amount;
        }
    }
    return balances;
}

inline int nextSequenceNumber(const std::string& path) {
    auto lines = readLines(path);
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
        JournalEntry e{};
        if (parseEntry(*it, e)) return e.sequence + 1;
    }
    return 1;
}

#endif /* Journal_h */


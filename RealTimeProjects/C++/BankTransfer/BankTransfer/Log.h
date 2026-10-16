//
//  Log.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Log_h
#define Log_h

#pragma once
#include <iostream>
#include <mutex>
#include <string>

inline std::mutex logMutex;

inline void log(const std::string& line) {
    std::lock_guard<std::mutex> guard(logMutex);
    std::cout << line << "\n";
}

#endif /* Log_h */

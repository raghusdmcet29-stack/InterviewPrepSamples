//
//  Backup.cpp
//  BackupStoreCPP
//
//  Created by Anussha on 18/11/26.
//

#include "Backup.hpp"
#include "ChunkStore.hpp"
#include <iostream>
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

int backupFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "could not open " << path << "\n";
        return 1;
    }
    std::cout << path << " is " << file.tellg() << " bytes\n";
    
    const std::streamsize chunkSize = 4096;
    std::vector<char> buffer(chunkSize);
    Store store;
    Order order;
    
    std::fstream storeFile("store.dat", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    long long storeEnd = 0;
    
    file.seekg(0, std::ios::beg);
    int chunkCount = 0;
    while (file) {
        file.read(buffer.data(), chunkSize);
        std::streamsize got = file.gcount();
        if (got > 0) {
            chunkCount++;
            std::string chunk(buffer.data(), got);
            std::size_t fingerprint = std::hash<std::string>{}(chunk);
            
            std::vector<ChunkLocation> &shelf = store[fingerprint];
            std::size_t position = shelf.size();
            for (std::size_t p = 0; p < shelf.size(); p++) {
                if (readChunk(storeFile, shelf[p]) == chunk) {
                    position = p;
                    break;
                }
            }
            if (position == shelf.size()) {
                storeFile.seekp(storeEnd);
                storeFile.write(chunk.data(), chunk.size());
                storeFile.flush();
                shelf.push_back(ChunkLocation{storeEnd, chunk.size()});
                storeEnd += chunk.size();
                std::cout << "chunk " << chunkCount << ": new, " << got << " bytes\n";
            }else {
                std::cout << "chunk " << chunkCount << ": duplicate\n";
            }
            order.emplace_back(fingerprint, position);
        }
    }
    
    std::size_t uniqueCount = 0;
    for (const auto &entry : store) {
        uniqueCount += entry.second.size();
    }
    std::cout << "total chunks: " << order.size()
                 << ", unique stored: " << uniqueCount << "\n";
    
    
    // save the reading list
    std::ofstream indexFile("store.idx");
    for (const auto &entry : order) {
        indexFile << entry.first << " " << entry.second << "\n";
    }
    indexFile.close();
    
    // save the catalog
    std::ofstream catalogFile("store.cat");
    for (const auto &entry : store) {
        for (const auto &loc : entry.second) {
            catalogFile << entry.first << " " << loc.offset << " " << loc.length << "\n";
        }
    }
    catalogFile.close();
    std::cout << "backup done. index saved to: " << fs::absolute("store.idx") << "\n";
    
  return 0;
}


//
//  Restore.cpp
//  BackupStoreCPP
//
//  Created by Anussha on 18/11/26.
//

#include "Restore.hpp"
#include "ChunkStore.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// load store.cat and store.idx back into memory
static int loadIndexes(Store &store, Order &order) {
    std::ifstream catFile("store.cat");
    if (!catFile) {
        std::cerr << "could not open store.cat\n";
        return 1;
    }
    std::size_t fingerprint;
    long long offset;
    std::size_t length;
    while (catFile >> fingerprint >> offset >> length) {
        store[fingerprint].push_back(ChunkLocation{offset, length});
    }
    
    std::ifstream idxFile("store.idx");
    if (!idxFile) {
        std::cerr << "could not open store.idx\n";
        return 1;
    }
    
    std::size_t position;
    while (idxFile >> fingerprint >> position) {
        order.emplace_back(fingerprint, position);
    }
    return 0;
}

int restoreFile(const std::string &destFolder, const std::string &originalPath) {
    Store store;
    Order order;
    if (loadIndexes(store, order) != 0) {
        return 1;
    }
    
    std::fstream storeFile("store.dat", std::ios::in | std::ios::binary);
    if (!storeFile) {
        std::cerr << "could not open store.dat\n";
        return 1;
    }
    
    fs::path outputPath = fs::path(destFolder) / (fs::path(originalPath).stem().string() + "_restored.txt");
    std::ofstream out(outputPath, std::ios::binary);
    if (!out) {
        std::cerr << "could not create " << outputPath << "\n";
        return 1;
    }
    
    for (const auto &entry : order) {
        const std::string piece = readChunk(storeFile, store.at(entry.first)[entry.second]);
        out.write(piece.data(), piece.size());
    }
    std::cout << "restored to " << outputPath << "\n";
    return 0;
}

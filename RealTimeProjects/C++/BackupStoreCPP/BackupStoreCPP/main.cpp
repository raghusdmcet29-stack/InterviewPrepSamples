//
//  main.cpp
//  BackupStoreCPP
//
//  Created by Anussha on 17/11/26.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <string>
#include <set>
#include <map>


int main(int argc, char* argv[]){
    
    if(argc < 2){
        std::cerr << "usage: backup <file>\n";
        return 1;
    }
    
    std::ifstream file(argv[1],std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "could not open " << argv[1] << "\n";
        return 1;
    }
    std::cout << argv[1] << " is " << file.tellg() << " bytes\n";
    
    const std::streamsize chunkSize = 4096;
    std::vector<char> buffer(chunkSize);
    
    
    // fingerprint -> list of distinct chunks that have this fingerprint
    std::map<std::size_t, std::vector<std::string>> store;
    // the recipe: for each chunk in the file, its fingerprint and its position on that shelf
    std::vector<std::pair<std::size_t, std::size_t>> order;
    
    file.seekg(0, std::ios::beg);
    int chunkCount = 0;
    while (file) {
        file.read(buffer.data(), chunkSize);
        std::streamsize got = file.gcount();
        if (got > 0) {
            chunkCount++;
            std::string chunk(buffer.data(), got);
            std::size_t fingerprint = std::hash<std::string>{}(chunk);
            
            std::vector<std::string> &shelf = store[fingerprint];
            std::size_t position = shelf.size();
            for (std::size_t p = 0; p < shelf.size(); p++) {
                if (shelf[p] == chunk) {
                    position = p;
                    break;
                }
            }
            if (position == shelf.size()) {
                shelf.push_back(chunk);
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
    
    
    std::string inputPath = argv[1];
    std::string base = inputPath;
    std::size_t dot = inputPath.find_last_of('.');
    std::size_t slash = inputPath.find_last_of('/');
    if (dot != std::string::npos && (slash == std::string::npos || dot > slash)) {
        base = inputPath.substr(0, dot);
    }
    std::string outputPath = base + "_restored.txt";
    
    std::ofstream out(outputPath, std::ios::binary);
    if (!out) {
        std::cerr << "could not create " << outputPath << "\n";
        return 1;
    }
    
    for (const auto &entry : order) {
        const std::string &piece = store.at(entry.first)[entry.second];
        out.write(piece.data(), piece.size());
    }
    std::cout << "wrote " << outputPath << "\n";
    return 0;
}

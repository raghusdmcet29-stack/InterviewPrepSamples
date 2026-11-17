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
    
    file.seekg(0, std::ios::beg);
    int chunkCount = 0;
    std::map<std::size_t, std::string> store;
    std::vector<std::size_t> order;
    while (file) {
        file.read(buffer.data(),chunkSize);
        std::streamsize got = file.gcount();
        if (got > 0) {
            chunkCount++;
            std::string chunk(buffer.data(), got);
            std::size_t fingerprint = std::hash<std::string>{}(chunk);
            order.push_back(fingerprint);
            
            if (store.count(fingerprint) == 0) {
                store[fingerprint] = chunk;
                std::cout << "chunk " << chunkCount << ": new, " << got << " bytes\n";
            }else {
                std::cout << "chunk " << chunkCount << ": duplicate\n";
            }
        }
    }
    std::cout << "total chunks: " << order.size()
              << ", unique stored: " << store.size() << "\n";
    
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
    
    for (std::size_t fingerprint : order) {
        const std::string &piece = store.at(fingerprint);
        out.write(piece.data(), piece.size());
    }
    std::cout << "wrote " << outputPath << "\n";
    return 0;
}

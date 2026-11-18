//
//  ChunkStore.cpp
//  BackupStoreCPP
//
//  Created by Anussha on 18/11/26.
//

#include "ChunkStore.hpp"

std::string readChunk(std::fstream &storeFile, const ChunkLocation &loc) {
    std::string bytes(loc.length, '\0');
    storeFile.seekg(loc.offset);
    storeFile.read(&bytes[0], loc.length);
    return bytes;
}

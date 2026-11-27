//
//  ChunkStore.hpp
//  BackupStoreCPP
//
//  Created by Anussha on 18/11/26.
//

#ifndef ChunkStore_hpp
#define ChunkStore_hpp

#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>

struct ChunkLocation {
    long long offset;   // where the chunk starts in store.dat
    size_t length;      // how many bytes it takes
};

using Store = std::map<std::size_t, std::vector<ChunkLocation>>;
using Order = std::vector<std::pair<std::size_t, std::size_t>>;

// read one stored chunk from store.dat
std::string readChunk(std::fstream &storeFile, const ChunkLocation &loc);

#endif /* ChunkStore_hpp */

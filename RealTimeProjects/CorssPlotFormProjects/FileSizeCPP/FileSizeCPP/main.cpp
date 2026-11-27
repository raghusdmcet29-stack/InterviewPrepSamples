//
//  main.cpp
//  FileSizeCPP
//
//  Created by Anussha on 27/11/26.
//

#include <iostream>
#include <fstream>


int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc < 2) {
        std::cerr<<"Usage : filessize <file>\n";
        return 1;
    }
    
    std::ifstream file(argv[1],std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr<<"Cannot open"<<argv[1]<<"\n";
        return 1;
    }
    
    std::cout<<"Size:"<<file.tellg()<<"bytes\n";
    return 0;
}

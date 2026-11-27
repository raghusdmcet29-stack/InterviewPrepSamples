#include "Backup.hpp"
#include "Restore.hpp"
#include <iostream>
#include <string>


int main() {
    std::string backupPath;
    std::cout << "Backup file path: ";
    std::cin >> backupPath;
    
    if (backupFile(backupPath) != 0) {
        return 1;
    }
    
    std::string destFolder;
    std::cout << "Restore destination folder: ";
    std::cin >> destFolder;
    return restoreFile(destFolder, backupPath);
}

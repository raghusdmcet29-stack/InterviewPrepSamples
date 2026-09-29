//
//  main.cpp
//  SingletonPattern
//
//  Created by Anussha on 11/09/26.
//

#include <iostream>
#include <string>

class AppConfig {
public:
    static AppConfig& shared(){
        static AppConfig instance;
        return instance;
    }
    
    std::string environment;
    int maxRetries;
    
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;
    
private:
    AppConfig() : environment("Production"), maxRetries(3) {}
};

int main() {
    AppConfig& a = AppConfig::shared();
    AppConfig& b = AppConfig::shared();

    std::cout << a.environment << std::endl;
    std::cout << a.maxRetries << std::endl;

    a.maxRetries = 10;
    std::cout << b.maxRetries << std::endl;
    std::cout << (&a == &b) << std::endl;
    //AppConfig c = AppConfig::shared(); // try this — what happens?
    //AppConfig d = AppConfig()
    return 0;
}

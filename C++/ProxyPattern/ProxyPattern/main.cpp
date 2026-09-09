//
//  main.cpp
//  ProxyPattern
//
//  Created by Anussha on 09/09/26.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <memory>

class DatabaseConnection {
public:
    virtual std::string query(const std::string& sql) = 0;
    virtual ~DatabaseConnection() = default;
};

class RealDatabaseConnection : public DatabaseConnection {
public:
    RealDatabaseConnection() {
        std::cout << "Connecting to database... (expensive setup)\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Connected.\n";
    }

    std::string query(const std::string& sql) override {
        return "Result of '" + sql + "'";
    }
};

class LazyDatabaseConnectionProxy : public DatabaseConnection {
private:
    std::unique_ptr<RealDatabaseConnection> realConnection;

public:
    std::string query(const std::string& sql) override {
        if (!realConnection) {
            std::cout << "Proxy: no real connection yet, creating one now...\n";
            realConnection = std::make_unique<RealDatabaseConnection>();
        }
        return realConnection->query(sql);
    }
};

int main() {
    std::cout << "Creating proxy...\n";
    std::unique_ptr<DatabaseConnection> proxyDb = std::make_unique<LazyDatabaseConnectionProxy>();
    std::cout << "Proxy created (no real connection yet)\n";

    std::cout << proxyDb->query("SELECT * FROM users") << "\n";
    std::cout << proxyDb->query("SELECT * FROM orders") << "\n";
}

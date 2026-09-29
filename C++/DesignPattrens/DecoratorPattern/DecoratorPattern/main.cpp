//
//  main.cpp
//  DecoratorPattern
//
//  Created by Anussha on 07/09/26.
//
#include <iostream>
#include <memory>
#include <string>
#include <optional>

class DataFetcher {
public:
    virtual ~DataFetcher() = default;
    virtual std::string fetch() = 0;
};

class BasicFetcher : public DataFetcher {
public:
    explicit BasicFetcher(std::string payload) : payload(std::move(payload)) {}
    std::string fetch() override { return payload; }
private:
    std::string payload;
};

class LoggingFetcher : public DataFetcher {
public:
    explicit LoggingFetcher(std::unique_ptr<DataFetcher> wrapped)
        : wrapped(std::move(wrapped)) {}

    std::string fetch() override {
        std::cout << "Fetching...\n";
        std::string result = wrapped->fetch();
        std::cout << "Fetched: " << result << "\n";
        return result;
    }

private:
    std::unique_ptr<DataFetcher> wrapped;
};

class CachingFetcher : public DataFetcher {
public:
    explicit CachingFetcher(std::unique_ptr<DataFetcher> wrapped)
        : wrapped(std::move(wrapped)) {}

    std::string fetch() override {
        if (cachedResult.has_value()) {
            std::cout << "Returning cached result\n";
            return cachedResult.value();
        }
        std::string result = wrapped->fetch();
        cachedResult = result;
        return result;
    }

private:
    std::unique_ptr<DataFetcher> wrapped;
    std::optional<std::string> cachedResult;
};

int main()
{
    
    auto fetcher = std::make_unique<LoggingFetcher>(
        std::make_unique<CachingFetcher>(
            std::make_unique<BasicFetcher>("user data")));

    std::cout << fetcher->fetch() << "\n";
    std::cout << "---\n";
    std::cout << fetcher->fetch() << "\n";
}

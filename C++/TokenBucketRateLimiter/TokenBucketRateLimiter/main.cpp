//
//  main.cpp
//  TokenBucketRateLimiter
//
//  Created by Anussha on 02/09/26.
//

#include <iostream>
//with out token bucket
/*void makeRequest(int id) {
    std::cout << "Request " << id << " allowed\n";
}

int main() {
    for (int i = 1; i <= 10; ++i) {
        makeRequest(i);
    }
    return 0;
}
*/

// with Token bucket

#include <chrono>

class TokenBucket {
public:
    TokenBucket(double maxTokens, double refillRatePerSecond)
        : maxTokens(maxTokens),
          tokens(maxTokens),
          refillRatePerSecond(refillRatePerSecond),
          lastRefillTime(std::chrono::steady_clock::now())
    {}

    bool tryConsume() {
        refill();
        if (tokens >= 1.0) {
            tokens -= 1.0;
            return true;
        }
        return false;
    }

private:
    void refill() {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - lastRefillTime;
        double newTokens = elapsed.count() * refillRatePerSecond;

        tokens = std::min(maxTokens, tokens + newTokens);
        lastRefillTime = now;
    }

    double maxTokens;
    double tokens;
    double refillRatePerSecond;
    std::chrono::steady_clock::time_point lastRefillTime;
};

#include <iostream>
#include <thread>

int main() {
    TokenBucket bucket(5.0, 5.0);  // max 5 tokens, refill 5 tokens/second

    std::cout << "--- Burst of 10 rapid requests ---\n";
    for (int i = 1; i <= 10; ++i) {
        if (bucket.tryConsume()) {
            std::cout << "Request " << i << " allowed\n";
        } else {
            std::cout << "Request " << i << " REJECTED\n";
        }
    }

    std::cout << "--- Waiting 1 second to refill ---\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "--- Trying again after refill ---\n";
    for (int i = 11; i <= 13; ++i) {
        if (bucket.tryConsume()) {
            std::cout << "Request " << i << " allowed\n";
        } else {
            std::cout << "Request " << i << " REJECTED\n";
        }
    }

    return 0;
}

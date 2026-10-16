//
//  main.cpp
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include "Bank.hpp"
#include "BoundedQueue.h"
#include "Barrier.h"
#include "TokenBucket.h"
#include "Log.h"
#include "Retry.h"
#include "AuditID.h"
#include "Summary.h"

void runTransfers(Bank& bank, int from, int to, long long amount, int count) {
    for (int i = 0; i < count; ++i) {
        bank.transfer({from, to, amount});
    }
}

void worker(Bank& bank, BoundedQueue& queue, Barrier& startLine,
            TokenBucket& bucket, int id) {
    auditID = "worker-" + std::to_string(id);   // this thread's own ID

    log("Worker " + std::to_string(id) + " arrived");
    startLine.wait();
    log("Worker " + std::to_string(id) + " released");
    while (true) {
        auto req = queue.dequeue();
        if (!req) break;
        bucket.take();
        withRetry(3, std::chrono::milliseconds(100), [&bank, &req]() {
            return bank.transfer(*req);
        });
        log("[" + auditID + "] moved " + std::to_string(req->amount)
            + " from " + std::to_string(req->from)
            + " to " + std::to_string(req->to));
    }
    log("Worker " + std::to_string(id) + " stopped");
}

int main() {
    Bank bank;
    bank.open(1, 1000);
    bank.open(2, 1000);

    // Stage 1: two threads transfer in opposite directions
    std::thread t1(runTransfers, std::ref(bank), 1, 2, 1, 1000);
    std::thread t2(runTransfers, std::ref(bank), 2, 1, 1, 1000);
    t1.join();
    t2.join();

    std::cout << "== Stage 1-4 results ==\n";
    std::cout << "A: " << bank.balanceOf(1) << ", B: " << bank.balanceOf(2)
              << ", total: " << bank.balanceOf(1) + bank.balanceOf(2) << "\n";
    std::cout << "Successful transfers: " << bank.successCount() << "\n";
    std::cout << "Failed transfers: " << bank.failedCount() << "\n";

    // Stage 5-7: worker pool with rate limit
    std::cout << "== Stage 5-7 ==\n";
    BoundedQueue queue(3);
    Barrier startLine(2);
    TokenBucket bucket(2, 10);

    auto start = std::chrono::steady_clock::now();

    std::thread w1(worker, std::ref(bank), std::ref(queue), std::ref(startLine),
                   std::ref(bucket), 1);
    std::thread w2(worker, std::ref(bank), std::ref(queue), std::ref(startLine),
                   std::ref(bucket), 2);

    for (int i = 0; i < 10; ++i) {
        queue.enqueue(TransferRequest{1, 2, 10});
    }
    queue.enqueue(std::nullopt);
    queue.enqueue(std::nullopt);

    w1.join();
    w2.join();

    std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
    std::cout << "Batch took " << std::fixed << std::setprecision(1)
              << elapsed.count() << " seconds\n";
    std::cout << "A: " << bank.balanceOf(1) << ", B: " << bank.balanceOf(2)
              << ", total: " << bank.balanceOf(1) + bank.balanceOf(2) << "\n";
    std::cout << "Successful transfers: " << bank.successCount() << "\n";
    std::cout << "Failed transfers: " << bank.failedCount() << "\n";
    
    // Stage 8 test: fails twice, then succeeds
    std::cout << "== Stage 8 test ==\n";
    int tries = 0;
    bool ok = withRetry(3, std::chrono::milliseconds(100), [&tries]() {
        ++tries;
        std::cout << "Attempt " << tries << "\n";
        return tries == 3;
    });
    std::cout << "Succeeded: " << std::boolalpha << ok
              << " after " << tries << " attempts\n";

    
    // Stage 10: summary with async tasks
    std::cout << "== Stage 10 summary ==\n";
    Summary s = summarize(bank, {1, 2});
    for (auto& [id, balance] : s.balances) {
        std::cout << "Account " << id << ": " << balance << "\n";
    }
    std::cout << "Total: " << s.total << "\n";
    std::cout << "Failed transfers: " << bank.failedCount() << "\n";
    
    return 0;
}

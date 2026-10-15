//
//  main.cpp
//  TicketBooking
//
//  Created by Anussha on 15/10/26.
//

#include <iostream>
#include <thread>
#include <vector>
#include <future>

std::mutex print_m;
std::mutex seats_m;
std::vector<bool> seats(3, false);   // false = free, true = booked


// Barrier: each thread waits until `total` threads have arrived.
std::mutex gate_m;
std::condition_variable gate_cv;
int arrived = 0;
const int total = 3;


void waitAtGate() {
    std::unique_lock lock(gate_m);
    arrived++;
    if (arrived == total) {
        gate_cv.notify_all();          // last one in opens the gate
    }else {
        gate_cv.wait(lock, [] { return arrived == total; });
    }
}

// Compare-and-swap counter: adds 1 only if the value is still what we read.
std::atomic<int> bookings{0};

void increment(std::atomic<int>& counter) {
    int old = counter.load();
    while (!counter.compare_exchange_strong(old, old + 1)) {
        // on failure, `old` now holds the current value, so we just retry
    }
}

// Thread-local storage: each thread keeps its own receipt number.
thread_local int receipt = 0;

struct BookingResult {
    int customer;
    int receipt;
    bool booked;
};

BookingResult bookSeat(int customer) {
    receipt = 100 + customer;
    waitAtGate();
    
    bool booked = false;
    std::lock_guard lock(seats_m);
    if (!seats[customer - 1]) {
        seats[customer - 1] = true;
        booked = true;
        increment(bookings);
    }
    return BookingResult{customer, receipt, booked};
}

int main() {
    std::vector<std::future<BookingResult>> futures;

    for (int customer = 1; customer <= total; customer++) {
        futures.push_back(std::async(std::launch::async, bookSeat, customer));
    }

    // Collect every result, then print the summary.
    std::vector<BookingResult> results;
    for (auto& f : futures) {
        results.push_back(f.get());
    }

    for (const auto& r : results) {
        std::cout << "customer " << r.customer << ": receipt " << r.receipt
                  << ", booked: " << (r.booked ? "true" : "false") << std::endl;
    }

    std::cout << "seats:";
    for (bool s : seats) {
        std::cout << " " << (s ? "true" : "false");
    }
    std::cout << std::endl;
    std::cout << "total bookings: " << bookings.load() << std::endl;
    return 0;
}

//
//  main.cpp
//  ThreadPool
//
//  Created by Anussha on 06/10/26.
//

// using q close and running parallel 3 task

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>
#include <optional>

std::mutex printMutex;

void printLine(const std::string& text) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << text << "\n";
}

struct Job {
    int id;

    void run() {
        printLine("Job " + std::to_string(id) + " started");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printLine("Job " + std::to_string(id) + " finished");
    }
};

class JobQueue {
private:
    std::queue<Job> jobs;
    bool closed = false;
    std::mutex m;
    std::condition_variable cv;

public:
    void add(Job job) {
        std::unique_lock<std::mutex> lock(m);
        jobs.push(job);
        cv.notify_one();
    }

    void close() {
        std::unique_lock<std::mutex> lock(m);
        closed = true;
        cv.notify_all();
    }

    std::optional<Job> take() {
        std::unique_lock<std::mutex> lock(m);
        while (jobs.empty() && !closed) {
            cv.wait(lock);
        }
        if (jobs.empty()) {
            return std::nullopt;
        }
        Job job = jobs.front();
        jobs.pop();
        return job;
    }
};

JobQueue queue;

void workerLoop(int w) {
    while (std::optional<Job> job = queue.take()) {
        job->run();
    }
    printLine("Worker " + std::to_string(w) + " quitting");
}

int main() {
    for (int id = 1; id <= 10; id++) {
        queue.add(Job{id});
    }
    queue.close();

    std::vector<std::thread> workers;
    for (int w = 1; w <= 3; w++) {
        workers.emplace_back(workerLoop, w);
    }
    for (std::thread& t : workers) {
        t.join();
    }
    std::cout << "Done\n";
    return 0;
}
/*
// using quew close concept sequenccial
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>
#include <optional>

std::mutex printMutex;

void printLine(const std::string& text) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << text << "\n";
}

struct Job {
    int id;

    void run() {
        printLine("Job " + std::to_string(id) + " started");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printLine("Job " + std::to_string(id) + " finished");
    }
};

class JobQueue {
private:
    std::queue<Job> jobs;
    bool closed = false;
    std::mutex m;
    std::condition_variable cv;

public:
    void add(Job job) {
        std::unique_lock<std::mutex> lock(m);
        jobs.push(job);
        cv.notify_one();
    }

    void close() {
        std::unique_lock<std::mutex> lock(m);
        closed = true;
        cv.notify_all();
    }

    std::optional<Job> take() {
        std::unique_lock<std::mutex> lock(m);
        while (jobs.empty() && !closed) {
            cv.wait(lock);
        }
        if (jobs.empty()) {
            return std::nullopt;
        }
        Job job = jobs.front();
        jobs.pop();
        return job;
    }
};

JobQueue queue;

void workerLoop() {
    while (std::optional<Job> job = queue.take()) {
        job->run();
    }
    printLine("Worker quitting");
}

int main() {
    queue.add(Job{1});
    queue.add(Job{2});
    queue.add(Job{3});
    queue.close();

    std::thread worker(workerLoop);
    worker.join();
    std::cout << "Done\n";
    return 0;
}
*/
// running parallel 3 jobs using worker group
/*
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>

std::mutex printMutex;

void printLine(const std::string& text) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << text << "\n";
}

struct Job {
    int id;

    void run() {
        printLine("Job " + std::to_string(id) + " started");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printLine("Job " + std::to_string(id) + " finished");
    }
};

class JobQueue {
private:
    std::queue<Job> jobs;
    std::mutex m;
    std::condition_variable cv;

public:
    void add(Job job) {
        std::unique_lock<std::mutex> lock(m);
        jobs.push(job);
        cv.notify_one();
    }

    Job take() {
        std::unique_lock<std::mutex> lock(m);
        while (jobs.empty()) {
            cv.wait(lock);
        }
        Job job = jobs.front();
        jobs.pop();
        return job;
    }
};

JobQueue queue;

void workerLoop() {
    for (int i = 0; i < 2; i++) {
        Job job = queue.take();
        job.run();
    }
}

int main() {
    for (int id = 1; id <= 6; id++) {
        queue.add(Job{id});
    }

    std::vector<std::thread> workers;
    for (int w = 0; w < 3; w++) {
        workers.emplace_back(workerLoop);
    }
    for (std::thread& t : workers) {
        t.join();
    }
    std::cout << "Done\n";
    return 0;
}
*/
// using worker loop instead of main loop
/*#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>

struct Job {
    int id;

    void run() {
        std::cout << "Job " << id << " started\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Job " << id << " finished\n";
    }
};

class JobQueue {
private:
    std::queue<Job> jobs;
    std::mutex m;
    std::condition_variable cv;

public:
    void add(Job job) {
        std::unique_lock<std::mutex> lock(m);
        jobs.push(job);
        cv.notify_one();
    }

    Job take() {
        std::unique_lock<std::mutex> lock(m);
        while (jobs.empty()) {
            cv.wait(lock);
        }
        Job job = jobs.front();
        jobs.pop();
        return job;
    }
};

JobQueue queue;

void workerLoop() {
    for (int i = 0; i < 3; i++) {
        Job job = queue.take();
        job.run();
    }
}

int main() {
    queue.add(Job{1});
    queue.add(Job{2});
    queue.add(Job{3});

    std::thread worker(workerLoop);
    worker.join();
    std::cout << "Done\n";
    return 0;
}
*/
// using main loop
/*
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>

struct Job {
    int id;

    void run() {
        std::cout << "Job " << id << " started\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Job " << id << " finished\n";
    }
};

class JobQueue {
private:
    std::queue<Job> jobs;
    std::mutex m;
    std::condition_variable cv;

public:
    void add(Job job) {
        std::unique_lock<std::mutex> lock(m);
        jobs.push(job);
        cv.notify_one();
    }

    Job take() {
        std::unique_lock<std::mutex> lock(m);
        while (jobs.empty()) {
            cv.wait(lock);
        }
        Job job = jobs.front();
        jobs.pop();
        return job;
    }
};

int main() {
    JobQueue queue;
    queue.add(Job{1});
    queue.add(Job{2});
    queue.add(Job{3});

    for (int i = 0; i < 3; i++) {
        Job job = queue.take();
        job.run();
    }
    std::cout << "Done\n";
    return 0;
}
*/
/*
#include <iostream>
#include <thread>
#include <chrono>

struct Job {
    int id;

    void run() {
        std::cout << "Job " << id << " started\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Job " << id << " finished\n";
    }
};

int main() {
    Job job{1};
    job.run();
    std::cout << "Done\n";
    return 0;
}
*/

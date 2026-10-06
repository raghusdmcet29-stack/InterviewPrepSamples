//
//  main.cpp
//  ReaderWriterLock
//
//  Created by Anussha on 06/10/26.
//

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <vector>

std::mutex printMutex;

void printLine(const std::string& text) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << text << "\n";
}

class Settings {
private:
    std::string theme = "light";
    std::shared_mutex rw;

public:
    std::string readTheme(int reader) {
        std::shared_lock<std::shared_mutex> lock(rw);
        printLine("Reader " + std::to_string(reader) + " started");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string value = theme;
        printLine("Reader " + std::to_string(reader) + " finished");
        return value;
    }
    void writeTheme(const std::string& newTheme) {
        std::unique_lock<std::shared_mutex> lock(rw);
        printLine("Writer started");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        theme = newTheme;
        printLine("Writer finished");
    }
};


Settings settings;

void readerTask(int reader) {
    std::string value = settings.readTheme(reader);
    printLine("Reader " + std::to_string(reader) + " saw " + value);
}

void writerTask() {
    settings.writeTheme("dark");
}

int main() {
    auto start = std::chrono::steady_clock::now();

    std::thread r1(readerTask, 1);
    std::thread r2(readerTask, 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::thread w(writerTask);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::thread r3(readerTask, 3);

    r1.join();
    r2.join();
    w.join();
    r3.join();

    double elapsed = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - start).count();
    std::cout << "Done in " << std::fixed << std::setprecision(1)
              << elapsed << " seconds\n";
    return 0;
}

/*Settings settings;

void readerTask(int reader) {
    settings.readTheme(reader);
}

int main() {
    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> readers;
    for (int r = 1; r <= 3; r++) {
        readers.emplace_back(readerTask, r);
    }
    for (std::thread& t : readers) {
        t.join();
    }

    double elapsed = std::chrono::duration<double>(
        std::chrono::steady_clock::now() - start).count();
    std::cout << "Done in " << std::fixed << std::setprecision(1)
              << elapsed << " seconds\n";
    return 0;
}
*/

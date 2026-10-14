//
//  main.cpp
//  ABAProblem2-VersionCounter,
//
//  Created by Anussha on 14/10/26.
//
//ABAProblem2-VersionCounter, part 5)

#include <atomic>
#include <cstdint>
#include <iostream>
#include <optional>
#include <thread>
#include <vector>

struct Node {
    int value;
    Node* next;
    Node(int v, Node* n = nullptr) : value(v), next(n) {}
};

struct TaggedTop {
    Node* node;
    std::uint64_t version;   // 64-bit so there is no padding
};

struct VersionedStack {
    std::atomic<TaggedTop> top;
    explicit VersionedStack(TaggedTop t) : top(t) {}
};

std::optional<int> pop(VersionedStack& stack) {
    TaggedTop current = stack.top.load();
    while (true) {
        if (current.node == nullptr) {
            return std::nullopt;
        }
        TaggedTop next{current.node->next, current.version + 1};
        // On failure, current is updated to the value now in top
        if (stack.top.compare_exchange_weak(current, next)) {
            return current.node->value;
        }
    }
}

int main() {
    Node* b = new Node(2);
    Node* a = new Node(1, b);
    VersionedStack stack(TaggedTop{a, 0});

    std::cout << "lock-free: " << stack.top.is_lock_free() << "\n";

    std::optional<int> results[2];
    std::vector<std::thread> threads;
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back([&stack, &results, i] {
            results[i] = pop(stack);
        });
    }
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "thread 0 popped: " << (results[0] ? *results[0] : -1) << "\n";
    std::cout << "thread 1 popped: " << (results[1] ? *results[1] : -1) << "\n";
    TaggedTop final_state = stack.top.load();
    std::cout << "top node value: " << (final_state.node ? final_state.node->value : -1) << "\n";
    std::cout << "top version: " << final_state.version << "\n";

    delete a;
    delete b;
    return 0;
}

//ABAProblem2-VersionCounter, part 4 (two threads)
/*
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

struct Node {
    int value;
    Node* next;
    Node(int v, Node* n = nullptr) : value(v), next(n) {}
};

struct TaggedTop {
    Node* node;
    int version;
};

struct VersionedStack {
    TaggedTop top;
    std::mutex m;

    TaggedTop load() {
        std::lock_guard<std::mutex> lock(m);
        return top;
    }

    bool compareAndSwap(TaggedTop expected, TaggedTop newTop) {
        std::lock_guard<std::mutex> lock(m);
        if (top.node == expected.node && top.version == expected.version) {
            top = newTop;
            return true;
        }
        return false;
    }
};

std::optional<int> pop(VersionedStack& stack) {
    while (true) {
        TaggedTop current = stack.load();
        if (current.node == nullptr) {
            return std::nullopt;
        }
        TaggedTop next{current.node->next, current.version + 1};
        if (stack.compareAndSwap(current, next)) {
            return current.node->value;
        }
    }
}

int main() {
    Node* b = new Node(2);
    Node* a = new Node(1, b);
    VersionedStack stack{TaggedTop{a, 0}};

    std::optional<int> results[2];
    std::vector<std::thread> threads;
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back([&stack, &results, i] {
            results[i] = pop(stack);
        });
    }
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "thread 0 popped: " << (results[0] ? *results[0] : -1) << "\n";
    std::cout << "thread 1 popped: " << (results[1] ? *results[1] : -1) << "\n";
    TaggedTop final_state = stack.load();
    std::cout << "top node value: " << (final_state.node ? final_state.node->value : -1) << "\n";
    std::cout << "top version: " << final_state.version << "\n";

    delete a;
    delete b;
    return 0;
}


*/

// ABAProblem2-VersionCounter, part 3
/*#include <iostream>
#include <optional>

struct Node {
    int value;
    Node* next;
    Node(int v, Node* n = nullptr) : value(v), next(n) {}
};

struct TaggedTop {
    Node* node;
    int version;
};

struct VersionedStack {
    TaggedTop top;

    bool compareAndSwap(TaggedTop expected, TaggedTop newTop) {
        if (top.node == expected.node && top.version == expected.version) {
            top = newTop;
            return true;
        }
        return false;
    }
};

// Pop with a retry loop: read fresh state, try the CAS, retry if it fails
std::optional<int> pop(VersionedStack& stack) {
    while (true) {
        TaggedTop current = stack.top;
        if (current.node == nullptr) {
            return std::nullopt;  // empty stack
        }
        TaggedTop next{current.node->next, current.version + 1};
        if (stack.compareAndSwap(current, next)) {
            return current.node->value;
        }
        // CAS failed: state changed, loop and read again
    }
}

int main() {
    Node* b = new Node(2);
    Node* a = new Node(1, b);
    VersionedStack stack{TaggedTop{a, 0}};

    // Thread 2's work, done up front for this single-threaded test:
    // pop A, pop B, push A back (version goes up each change)
    stack.top = TaggedTop{b, 1};
    stack.top = TaggedTop{nullptr, 2};
    a->next = nullptr;
    stack.top = TaggedTop{a, 3};

    // Thread 1 pops now, reading fresh state
    std::optional<int> popped = pop(stack);
    std::cout << "popped value: " << (popped ? *popped : -1) << "\n";
    std::cout << "top node value: " << (stack.top.node ? stack.top.node->value : -1) << "\n";
    std::cout << "top version: " << stack.top.version << "\n";

    delete a;
    delete b;
    return 0;
}

*/




//ABAProblem2-VersionCounter, part 2
/*
#include <iostream>

struct Node {
    int value;
    Node* next;
    Node(int v, Node* n = nullptr) : value(v), next(n) {}
};

struct TaggedTop {
    Node* node;
    int version;
};

struct VersionedStack {
    TaggedTop top;

    // Succeeds only if BOTH the node and the version match
    bool compareAndSwap(TaggedTop expected, TaggedTop newTop) {
        if (top.node == expected.node && top.version == expected.version) {
            top = newTop;
            return true;
        }
        return false;
    }
};

int main() {
    Node* b = new Node(2);
    Node* a = new Node(1, b);
    VersionedStack stack{TaggedTop{a, 0}};

    // Thread 1 saves the top and its next
    TaggedTop saved = stack.top;             // (A, 0)
    Node* savedNext = saved.node->next;      // B

    // Thread 2 pops A, pops B, pushes A back (version goes up each change)
    stack.top = TaggedTop{b, 1};             // pop A
    stack.top = TaggedTop{nullptr, 2};       // pop B
    a->next = nullptr;
    stack.top = TaggedTop{a, 3};             // push A back

    // Thread 1 resumes and tries its CAS with the old value
    bool succeeded = stack.compareAndSwap(saved, TaggedTop{savedNext, saved.version + 1});
    std::cout << "CAS succeeded: " << (succeeded ? "false" : "false") << "\n";
    std::cout << "top node value: " << stack.top.node->value << "\n";
    std::cout << "top version: " << stack.top.version << "\n";

    delete a;
    delete b;
    return 0;
}

*/


//ABAProblem2-VersionCounter, part 1
/*#include <iostream>

struct Node {
    int value;
    Node* next;
    Node(int v, Node* n = nullptr) : value(v), next(n) {}
};

// The pointer and the version travel together as one value
struct TaggedTop {
    Node* node;
    int version;
};

int main() {
    Node* b = new Node(2);
    Node* a = new Node(1, b);

    TaggedTop start{a, 0};
    std::cout << "node value: " << start.node->value << "\n";
    std::cout << "version: " << start.version << "\n";

    delete a;
    delete b;
    return 0;
}
*/

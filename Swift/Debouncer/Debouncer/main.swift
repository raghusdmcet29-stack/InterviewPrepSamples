//
//  main.swift
//  Debouncer
//
//  Created by Anussha on 02/09/26.
//

import Foundation
/*
//The naive version (no debouncing at all)
final class NaiveSearcher {
    func onTextChanged(_ text: String) {
        search(for: text)
    }

    private func search(for text: String) {
        print("Searching for: \(text)")
    }
}

let searcher = NaiveSearcher()
//Test it — simulate fast typing by calling this 5 times in a row, once per letter of "hello":
for letter in ["h", "he", "hel", "hell", "hello"] {
    searcher.onTextChanged(letter)
}

*/

final class Debouncer {
    private var pendingTask: Task<Void, Never>?
    private let delay: Duration

    init(delay: Duration) {
        self.delay = delay
    }

    func call(action: @escaping () -> Void) {
        pendingTask?.cancel()

        pendingTask = Task {
            try? await Task.sleep(for: delay)
            guard !Task.isCancelled else { return }
            action()
        }
    }
}

final class DebouncedSearcher {
    private let debouncer = Debouncer(delay: .seconds(1))

    func onTextChanged(_ text: String) {
        debouncer.call {
            print("Searching for: \(text)")
        }
    }
}

/*let searcher = DebouncedSearcher()

for letter in ["h", "he", "hel", "hell", "hello"] {
    searcher.onTextChanged(letter)
}

// Keep the program alive long enough for the debounced task to fire
try await Task.sleep(for: .seconds(2))
print("Done waiting")*/

let searcher = DebouncedSearcher()

searcher.onTextChanged("h")
try await Task.sleep(for: .seconds(1.2))  // pause longer than the 1-second delay
searcher.onTextChanged("he")
try await Task.sleep(for: .seconds(1.2))  // pause longer than the 1-second delay
searcher.onTextChanged("hel")



searcher.onTextChanged("hell")
searcher.onTextChanged("hello")

try await Task.sleep(for: .seconds(2))
print("Done waiting")

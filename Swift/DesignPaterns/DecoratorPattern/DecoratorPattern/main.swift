//
//  main.swift
//  DecoratorPattern
//
//  Created by Anussha on 07/09/26.
//

import Foundation

protocol DataFetcher {
    func fetch() -> String
}

struct BasicFetcher: DataFetcher {
    let payload: String
    func fetch() -> String {
        return payload
    }
}

struct LoggingFetcher: DataFetcher {
    let wrapped: DataFetcher

    func fetch() -> String {
        print("Fetching...")
        let result = wrapped.fetch()
        print("Fetched: \(result)")
        return result
    }
}

final class CachingFetcher: DataFetcher {
    let wrapped: DataFetcher
    private var cachedResult: String?

    init(wrapped: DataFetcher) {
        self.wrapped = wrapped
    }

    func fetch() -> String {
        if let cached = cachedResult {
            print("Returning cached result")
            return cached
        }
        let result = wrapped.fetch()
        cachedResult = result
        return result
    }
}

let fetcher: DataFetcher = LoggingFetcher(wrapped: CachingFetcher(wrapped: BasicFetcher(payload: "user data")))

print(fetcher.fetch())   // first call — no cache yet
print("---")
print(fetcher.fetch())   // second call — should hit cache


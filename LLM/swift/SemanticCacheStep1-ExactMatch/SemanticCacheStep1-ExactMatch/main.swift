//
//  main.swift
//  SemanticCacheStep1-ExactMatch
//
//  Created by Anussha on 08/10/26.
//

var cache: [String: String] = [:]

func fakeModel(_ question: String) -> String {
    print("  MODEL CALL for: \(question)")
    return "Paris"
}

func ask(_ question: String) -> String {
    if let saved = cache[question] {
        print("  CACHE HIT")
        return saved
    }
    let answer = fakeModel(question)
    cache[question] = answer
    return answer
}

print(ask("What is the capital of France?"))
print(ask("What is the capital of France?"))
print(ask("what's the capital of france"))


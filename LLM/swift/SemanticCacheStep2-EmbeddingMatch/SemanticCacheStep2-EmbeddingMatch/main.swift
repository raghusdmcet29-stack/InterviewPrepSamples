//
//  main.swift
//  SemanticCacheStep2-EmbeddingMatch
//
//  Created by Anussha on 08/10/26.
//

import Foundation
import NaturalLanguage
func cosineSimilarity(_ a: [Double], _ b: [Double]) -> Double {
    var dot = 0.0
    var normA = 0.0
    var normB = 0.0
    for i in 0..<a.count {
        dot += a[i] * b[i]
        normA += a[i] * a[i]
        normB += b[i] * b[i]
    }
    return dot / (normA.squareRoot() * normB.squareRoot())
}

struct CacheEntry {
    let question: String
    let embedding: [Double]
    let answer: String
}

var entries = [
    CacheEntry(question: "What is the capital of France?", embedding: [1.0, 0.0], answer: "Paris"),
    CacheEntry(question: "How tall is Mount Everest?", embedding: [0.0, 1.0], answer: "8849 m")
]

let threshold = 0.80

func bestMatch(for query: [Double]) -> (entry: CacheEntry, score: Double)? {
    var bestEntry: CacheEntry? = nil
    var bestScore = -1.0
    for entry in entries {
        let score = cosineSimilarity(query, entry.embedding)
        if score > bestScore {
            bestScore = score
            bestEntry = entry
        }
    }
    if let found = bestEntry {
        return (found, bestScore)
    }
    return nil
}

func fakeModel(_ question: String) -> String {
    print("  MODEL CALL for: \(question)")
    return "Spain answer"
}

func ask(_ question: String, _ embedding: [Double]) -> String {
    if let match = bestMatch(for: embedding), match.score > threshold {
        print("  CACHE HIT (score \(match.score))")
        return match.entry.answer
    }
    let answer = fakeModel(question)
    entries.append(CacheEntry(question: question, embedding: embedding, answer: answer))
    return answer
}

func localEmbedding(_ text: String) -> [Double] {
    guard let model = NLEmbedding.sentenceEmbedding(for: .english),
          let vector = model.vector(for: text) else {
        fatalError("Sentence embedding not available")
    }
    return vector
}

let e1 = localEmbedding("What is the capital of France?")
let e2 = localEmbedding("Which city is France's capital?")
let e3 = localEmbedding("What is the capital of Spain?")

print("vector length:", e1.count)
print("France vs France reworded:", cosineSimilarity(e1, e2))
print("France vs Spain:", cosineSimilarity(e1, e3))
print(ask("Which city is France's capital?", [0.9, 0.1]))
print(ask("Tell me about Spain", [0.5, 0.5]))
print(ask("Tell me about Spain", [0.5, 0.5]))



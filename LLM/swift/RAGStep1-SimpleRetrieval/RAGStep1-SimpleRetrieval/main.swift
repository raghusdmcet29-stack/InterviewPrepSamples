//
//  main.swift
//  RAGStep1-SimpleRetrieval
//
//  Created by Anussha on 09/09/26.
//

import Foundation

struct GroqResponse: Codable {
    let choices: [Choice]
}

struct Choice: Codable {
    let message: Message
}

struct Message: Codable {
    let content: String
}

let documents = [
    "Our refund policy allows returns within 30 days.",
    "Our office is located in Bangalore.",
    "Shipping takes 3-5 business days."
]


func overlapScore(question: String, document: String) -> Int {
    let questionWords = Set(question.lowercased().split(separator: " "))
    let documentWords = Set(document.lowercased().split(separator: " "))
    return questionWords.intersection(documentWords).count
}

func findBestMatch(question: String, documents: [String]) -> String {
    var bestDocument = documents[0]
    var bestScore = 0

    for document in documents {
        let score = overlapScore(question: question, document: document)
        if score > bestScore {
            bestScore = score
            bestDocument = document
        }
    }

    return bestDocument
}

func callGroq(prompt: String) async throws -> String {
    let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"]!

    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "messages": [["role": "user", "content": prompt]],
        "temperature": 0.2
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (data, _) = try await URLSession.shared.data(for: request)
    let response = try JSONDecoder().decode(GroqResponse.self, from: data)
    return response.choices[0].message.content
}

let question = "How long for a refund"
let bestMatch = findBestMatch(question: question, documents: documents)

print("Question: \(question)")
print("Best matching document: \(bestMatch)")

let prompt = """
Context: \(bestMatch)

Question: \(question)

Answer using only the context above.
"""


Task {
    do {
        let answer = try await callGroq(prompt: prompt)
        print("LLM Answer: \(answer)")
    
    } catch {
        print("Error: \(error)")
       
    }
    exit(0)
}

RunLoop.main.run()

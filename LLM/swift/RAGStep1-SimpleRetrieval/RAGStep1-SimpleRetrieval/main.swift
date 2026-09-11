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
    "Our office is located in Bangalore.",
    "Our refund policy allows returns within 30 days.",
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

func getEmbedding(text: String) async throws -> [Double] {
    let apiKey = ProcessInfo.processInfo.environment["HF_API_KEY"] ?? ""
    let url = URL(string: "https://router.huggingface.co/hf-inference/models/sentence-transformers/all-MiniLM-L6-v2/pipeline/feature-extraction")!

    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    request.httpBody = try JSONSerialization.data(withJSONObject: ["inputs": text])

    let (data, response) = try await URLSession.shared.data(for: request)

    if let httpResponse = response as? HTTPURLResponse {
        print("Status code: \(httpResponse.statusCode)")
    }
    let embedding = try JSONDecoder().decode([Double].self, from: data)
    return embedding
}

func cosineSimilarity(a: [Double], b: [Double]) -> Double {
    var dotProduct = 0.0
    var magnitudeA = 0.0
    var magnitudeB = 0.0

    for i in 0..<a.count {
        dotProduct += a[i] * b[i]
        magnitudeA += a[i] * a[i]
        magnitudeB += b[i] * b[i]
    }

    magnitudeA = sqrt(magnitudeA)
    magnitudeB = sqrt(magnitudeB)

    return dotProduct / (magnitudeA * magnitudeB)
}

func findBestMatchByEmbedding(question: String, documents: [String]) async throws -> String {
    let questionEmbedding = try await getEmbedding(text: question)

    var bestDocument = documents[0]
    var bestScore = -1.0

    for document in documents {
        let documentEmbedding = try await getEmbedding(text: document)
        let score = cosineSimilarity(a: questionEmbedding, b: documentEmbedding)

        if score > bestScore {
            bestScore = score
            bestDocument = document
        }
    }

    return bestDocument
}

let question = "Can I send back a product I don't want?" /*"How long for a refund?"*/

Task {
    do {
        let bestMatch = try await findBestMatchByEmbedding(question: question, documents: documents)

        print("Question: \(question)")
        print("Best matching document: \(bestMatch)")

        let prompt = """
        Context: \(bestMatch)

        Question: \(question)

        Answer using only the context above.
        """

        let answer = try await callGroq(prompt: prompt)
        print("LLM Answer: \(answer)")
    } catch {
        print("Error: \(error)")
    }
    exit(0)
}

RunLoop.main.run()

/*
let question = "How long for a refund"
let bestMatch = findBestMatch(question: question, documents: documents)

print("Question: \(question)")
print("Best matching document: \(bestMatch)")

// Temporarily testing getEmbedding in isolation — pipeline below not used yet
Task {
    do {
        let vec = try await getEmbedding(text: "How long does a refund take?")
        print("Vector length: \(vec.count)")
        print("First 5 values: \(vec.prefix(5))")
    } catch {
        print("Error: \(error)")
    }
    exit(0)
}
*/
/*let d: [Double] = [2, 0]
let e: [Double] = [3, 0]

let similarity = cosineSimilarity(a: d, b: e)
print("Similarity between D and E: \(similarity)")
*/

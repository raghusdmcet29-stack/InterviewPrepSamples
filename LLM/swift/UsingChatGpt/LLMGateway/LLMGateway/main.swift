//
//  main.swift
//  LLMGateway
//
//  Created by Anussha on 08/10/26.
//

import Foundation

struct LLMResponse {
    let answer: String
    let latency: TimeInterval
    let statusCode: Int
}

enum LLMError: Error {
    case missingAPIKey
    case invalidResponse
    case httpError(Int)
    case invalidJSON
}

extension LLMError: LocalizedError {

    var errorDescription: String? {

        switch self {

        case .missingAPIKey:
            return "GROQ_API_KEY is not set"

        case .invalidResponse:
            return "Invalid response from LLM API"

        case .httpError(let statusCode):
            return "LLM API returned HTTP \(statusCode)"

        case .invalidJSON:
            return "Could not parse LLM API response"
        }
    }
}

func callGroq(question: String) async throws -> LLMResponse{

    guard let key = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
        print("ERROR: GROQ_API_KEY is not set")
        throw LLMError.missingAPIKey
    }

    let url = URL(
        string: "https://api.groq.com/openai/v1/chat/completions"
    )!

    var request = URLRequest(url: url)
    request.httpMethod = "POST"

    request.setValue(
        "Bearer \(key)",
        forHTTPHeaderField: "Authorization"
    )

    request.setValue(
        "application/json",
        forHTTPHeaderField: "Content-Type"
    )

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "messages": [
            [
                "role": "user",
                "content": question
            ]
        ]
    ]

    do {

        request.httpBody = try JSONSerialization.data(
            withJSONObject: body
        )

        let start = Date()

        let (data, response) = try await URLSession.shared.data(
            for: request
        )

        let latency = Date().timeIntervalSince(start)

        let statusCode =
            (response as? HTTPURLResponse)?.statusCode ?? 0

        print("HTTP status:", statusCode)

        let json = try JSONSerialization.jsonObject(
            with: data
        ) as! [String: Any]

        let choices = json["choices"] as! [[String: Any]]

        let message =
            choices[0]["message"] as! [String: Any]

        let answer =
            message["content"] as! String

        return LLMResponse(
            answer: answer,
            latency: latency,
            statusCode: statusCode
        )

    } catch {

        print("ERROR:", error.localizedDescription)
        throw error
    }
}

func callGroqWithRetry(question: String) async -> LLMResponse? {

    for attempt in 1...3 {

        do {
            return try await callGroq(question: question)
        } catch {
            print("Attempt \(attempt) failed:", error.localizedDescription)
        }
    }

    return nil
}


print("LLM Gateway")

print("============")

let result = await callGroqWithRetry(
    question: "What is the capital of France?"
)

print("")

print("LLM Answer:")

print(result?.answer ?? "No answer")

print("")

print("HTTP Status:", result?.statusCode ?? 0)

if let latency = result?.latency {
    print(
        "Latency:",
        String(format: "%.3f", latency),
        "seconds"
    )
}

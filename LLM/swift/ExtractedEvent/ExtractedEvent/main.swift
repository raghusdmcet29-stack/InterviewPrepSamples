//
//  main.swift
//  ExtractedEvent
//
//  Created by Anussha on 01/09/26.
//

import Foundation

struct ExtractedEvent: Codable {
    let eventName: String
    let date: String
    let location: String?
    let attendeeCount: Int?
}

func callGroq(prompt: String) async throws -> String {
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "temperature": 0.2,
        "messages": [["role": "user", "content": prompt]]
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (data, _) = try await URLSession.shared.data(for: request)
    return String(data: data, encoding: .utf8) ?? ""
}

struct GroqResponse: Codable {
    struct Choice: Codable {
        struct Message: Codable {
            let content: String
        }
        let message: Message
    }
    let choices: [Choice]
}

func parseExtractedEvent(from rawResponse: String) throws -> ExtractedEvent {
    let groqData = rawResponse.data(using: .utf8)!
    let groqResponse = try JSONDecoder().decode(GroqResponse.self, from: groqData)

    let innerJSON = groqResponse.choices[0].message.content
    let innerData = innerJSON.data(using: .utf8)!

    return try JSONDecoder().decode(ExtractedEvent.self, from: innerData)
}

let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] ?? ""

let prompt = """
Extract event details as JSON with keys: eventName, date, location, attendeeCount.
Text: "The product launch is on March 15th in Bangalore, expecting 200 people."
Respond with ONLY the JSON object, no other text.
"""

Task {
    do {
        let raw = try await callGroq(prompt: prompt)
        let event = try parseExtractedEvent(from: raw)
        print("Success: \(event)")
    } catch {
        print("Failed: \(error)")
    }
    exit(0)
}

RunLoop.main.run()


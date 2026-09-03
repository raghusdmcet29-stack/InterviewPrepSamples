//
//  main.swift
//  StreamingResponseParser
//
//  Created by Anussha on 03/09/26.
//

import Foundation

guard let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
    fatalError("Set GROQ_API_KEY environment variable before running")
}

let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!

func streamChat(prompt: String) async throws {
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "messages": [["role": "user", "content": prompt]],
        "stream": true
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (bytes, _) = try await URLSession.shared.bytes(for: request)

    var chunkNumber = 0

    for try await line in bytes.lines {
        guard line.hasPrefix("data: ") else { continue }
        let jsonString = String(line.dropFirst("data: ".count))

        if jsonString == "[DONE]" {
            print("\n--- Stream finished (\(chunkNumber) content chunks total) ---")
            break
        }

        guard let jsonData = jsonString.data(using: .utf8),
              let json = try? JSONSerialization.jsonObject(with: jsonData) as? [String: Any],
              let choices = json["choices"] as? [[String: Any]],
              let delta = choices.first?["delta"] as? [String: Any],
              let content = delta["content"] as? String else {
            continue
        }

        chunkNumber += 1
        print("[chunk \(chunkNumber)] content: \"\(content)\"")
    }
}

// Usage:
try await streamChat(prompt: "Count from 1 to 5.")


//
//  main.swift
//  ConversationMemoryStep3-TokenTrimming
//
//  Created by Anussha on 17/09/26.
//

import Foundation

// MARK: - Message type
struct ChatMessage {
    let role: String
    let content: String
}

func countTokens(_ text: String) -> Int {
    return text.split(separator: " ").count
}

func applyTokenTrimming(_ messages: [ChatMessage], maxTokens: Int) -> [ChatMessage] {
    var totalTokens = messages.reduce(0) { $0 + countTokens($1.content) }
    var trimmed = messages

    while totalTokens > maxTokens && !trimmed.isEmpty {
        let removed = trimmed.removeFirst()
        totalTokens -= countTokens(removed.content)
    }

    return trimmed
}

// MARK: - Core API call (same as Step 1)
func callGroq(messages: [ChatMessage]) async throws -> String {
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    guard let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
        fatalError("GROQ_API_KEY not set")
    }
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")

    let payload: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "temperature": 0.2,
        "messages": messages.map { ["role": $0.role, "content": $0.content] }
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: payload)

    let (data, _) = try await URLSession.shared.data(for: request)
    let json = try JSONSerialization.jsonObject(with: data) as! [String: Any]
    let choices = json["choices"] as! [[String: Any]]
    let message = choices[0]["message"] as! [String: String]
    return message["content"] ?? ""
}


let liveHistory = [
    ChatMessage(role: "user", content: "My name is Anuraag and I live in Bangalore"),
    ChatMessage(role: "user", content: "I work as a Technical Lead in macOS development"),
    ChatMessage(role: "assistant", content: "That's great, macOS development is a solid specialty"),
    ChatMessage(role: "user", content: "What's my name?")
]

let liveTrimmed = applyTokenTrimming(liveHistory, maxTokens: 30)

print("Messages sent to model:")
for msg in liveTrimmed {
    print("  \(msg.role): \(msg.content)")
}

Task {
    let answer = try await callGroq(messages: liveTrimmed)
    print("Model answer: \(answer)")
    exit(0)
}
RunLoop.main.run()



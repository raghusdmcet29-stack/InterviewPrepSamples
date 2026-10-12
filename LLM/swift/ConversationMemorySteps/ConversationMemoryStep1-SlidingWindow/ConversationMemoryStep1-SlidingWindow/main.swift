//
//  main.swift
//  ConversationMemoryStep1-SlidingWindow
//
//  Created by Anussha on 15/09/26.
//

import Foundation

struct ChatMessage {
    let role: String    // "user" or "assistant"
    let content: String
}

func applySlidingWindow(_ messages: [ChatMessage], maxWindow: Int) -> [ChatMessage] {
    if messages.count <= maxWindow {
        return messages
    }
    return Array(messages.suffix(maxWindow))
}

import Foundation

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

func runTest() async {
    var messages: [ChatMessage] = [
        ChatMessage(role: "user", content: "Hi, my name is Anuraag"),
        ChatMessage(role: "assistant", content: "Hello Anuraag!"),
        ChatMessage(role: "user", content: "What's 2+2?"),
        ChatMessage(role: "assistant", content: "4"),
        ChatMessage(role: "user", content: "What's 10x10?"),
        ChatMessage(role: "assistant", content: "100")
    ]

    // New question that depends on the FIRST message
    messages.append(ChatMessage(role: "user", content: "What's my name?"))

    let trimmed = applySlidingWindow(messages, maxWindow: 4)

    print("--- Trimmed history being sent ---")
    for msg in trimmed {
        print("\(msg.role): \(msg.content)")
    }

    do {
        let answer = try await callGroq(messages: trimmed)
        print("--- Model's answer ---")
        print(answer)
    } catch {
        print("Error: \(error)")
    }
}

// Blocking wrapper (same pattern as prior exercises)
let semaphore = DispatchSemaphore(value: 0)
Task {
    await runTest()
    semaphore.signal()
}
semaphore.wait()

/*
let messages: [ChatMessage] = [
    ChatMessage(role: "user", content: "Hi"),
    ChatMessage(role: "assistant", content: "Hello!"),
    ChatMessage(role: "user", content: "What's 2+2?"),
    ChatMessage(role: "assistant", content: "4"),
    ChatMessage(role: "user", content: "What's 10x10?"),
    ChatMessage(role: "assistant", content: "100")
]

let trimmed = applySlidingWindow(messages, maxWindow: 4)

for msg in trimmed {
    print("\(msg.role): \(msg.content)")
}
*/

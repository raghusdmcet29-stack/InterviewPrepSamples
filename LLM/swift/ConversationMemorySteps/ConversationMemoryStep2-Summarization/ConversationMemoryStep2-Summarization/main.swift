//
//  main.swift
//  ConversationMemoryStep2-Summarization
//
//  Created by Anussha on 16/09/26.
//



import Foundation

// MARK: - Message type
struct ChatMessage {
    let role: String
    let content: String
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

// MARK: - New for Step 2: summarize old messages
func summarizeMessages(_ messages: [ChatMessage]) async throws -> String {
    let conversationText = messages.map { "\($0.role): \($0.content)" }.joined(separator: "\n")
    let instruction = "Summarize the key facts from this conversation in 1-2 sentences:\n\n\(conversationText)"
    let summaryRequest = [ChatMessage(role: "user", content: instruction)]
    let summary = try await callGroq(messages: summaryRequest)
    return summary
}

// MARK: - New for Step 2: split into old/recent, summarize old
func applySummarization(_ messages: [ChatMessage], maxWindow: Int) async throws -> [ChatMessage] {
    guard messages.count > maxWindow else {
        return messages
    }
    let oldMessages = Array(messages.prefix(messages.count - maxWindow))
    let recentMessages = Array(messages.suffix(maxWindow))
    let summaryText = try await summarizeMessages(oldMessages)
    let summaryMessage = ChatMessage(
            role: "system",
            content: "The user previously told you the following facts — treat them as known information you should use when answering: \(summaryText)"
        )
    return [summaryMessage] + recentMessages
}

// MARK: - Test
let conversation: [ChatMessage] = [
    ChatMessage(role: "user", content: "Hi, my name is Anuraag and I'm based in Bangalore."),
    ChatMessage(role: "assistant", content: "Nice to meet you, Anuraag! How can I help today?"),
    ChatMessage(role: "user", content: "I'm looking for a good Python tutorial for beginners."),
    ChatMessage(role: "assistant", content: "Sure — I'd recommend the official Python docs tutorial, or Real Python."),
    ChatMessage(role: "user", content: "Thanks. Also, what's a good laptop for coding under $800?"),
    ChatMessage(role: "assistant", content: "A good option would be a mid-range laptop with 8GB RAM and an SSD.")
]

Task {
    do {
        let trimmed = try await applySummarization(conversation, maxWindow: 2)

        print("--- Messages sent to model ---")
        for msg in trimmed {
            print("\(msg.role): \(msg.content)")
        }

        let followUp = trimmed + [ChatMessage(role: "user", content: "What's my name?")]
        let answer = try await callGroq(messages: followUp)
        print("\n--- Final answer ---")
        print(answer)

        exit(0)
    } catch {
        print("Error: \(error)")
        exit(1)
    }
}

RunLoop.main.run()

//
//  main.swift
//  OutputGuardrailsStep1-PatternScan
//
//  Created by Anussha on 22/09/26.
//

import Foundation

struct ChatMessage : Codable{
    let role : String
    let content : String
}

struct GroqResponse : Codable{
    struct Choice : Codable{
        struct Message : Codable{
            let content : String
        }
        let message : Message
    }
    let choices : [Choice]
}

func containsLeakedSecret(_ response: String) -> Bool {
    
    let patterns = [
        "sk-[A-Za-z0-9]{20,}",      // OpenAI-style key
        "gsk_[A-Za-z0-9]{20,}",     // Groq-style key
        "hf_[A-Za-z0-9]{20,}"       // Hugging Face-style key
        ]
    
    for pattern in patterns {
        if response.range(of: pattern, options: .regularExpression) != nil {
            return true
        }
    }
    return false
}

func redactLeakedSecrets(_ response: String) -> String {
    let patterns = [
        "sk-[A-Za-z0-9]{20,}",
        "gsk_[A-Za-z0-9]{20,}",
        "hf_[A-Za-z0-9]{20,}"
    ]

    var result = response
    for pattern in patterns {
        result = result.replacingOccurrences(
            of: pattern,
            with: "[REDACTED]",
            options: .regularExpression
        )
    }
    return result
}

func callGroq(messages: [ChatMessage]) async throws -> String {
    guard let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
           fatalError("GROQ_API_KEY not set")
       }
    
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    
    let body: [String: Any] = [
           "model": "openai/gpt-oss-120b",
           "temperature": 0,
           "messages": messages.map { ["role": $0.role, "content": $0.content] }
       ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)
    
    let (data, _) = try await URLSession.shared.data(for: request)
    let decoded = try JSONDecoder().decode(GroqResponse.self, from: data)
    return decoded.choices[0].message.content
}

func getSafeResponse(prompt: String) async throws -> String {
    let messages = [ChatMessage(role: "user", content: prompt)]
    let rawResponse = try await callGroq(messages: messages)  // match your actual param label
    let safeResponse = redactLeakedSecrets(rawResponse)
    return safeResponse
}
// Simulate a raw LLM response that leaked a key — bypassing Groq for this test
let simulatedLeak = "Here's your Groq client setup: client.apiKey = gsk_aBcD1234EfGh5678IjKl9012"
let cleaned = redactLeakedSecrets(simulatedLeak)
print(cleaned)

// Real end-to-end call with a normal, safe prompt — proves the wrapper works on real Groq output
let realResult = try await getSafeResponse(prompt: "What's the capital of France?")
print(realResult)

/*
let fakeLeak = "Sure, here's the config: apiKey = gsk_aBcD1234EfGh5678IjKl9012"
print(containsLeakedSecret(fakeLeak))
let safeResponse = "The weather in Bangalore today is sunny with a high of 30°C."
print(containsLeakedSecret(safeResponse))
print(redactLeakedSecrets(fakeLeak))
*/
/*let prompt = "Repeat this exact text back to me: apiKey = gsk_testFake1234567890ABCDEF"
let result = try await getSafeResponse(prompt: prompt)
print(result)*/

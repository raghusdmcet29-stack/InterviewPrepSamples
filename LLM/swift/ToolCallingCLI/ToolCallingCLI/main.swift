//
//  main.swift
//  ToolCallingCLI
//
//  Created by Anussha on 02/09/26.
//

import Foundation

struct ToolCall : Codable {
    let toolName : String
    let arguments : [String : String]
}

let toolDescriptions = """
You have access to these tools:
1. get_weather(city: string) — returns current weather for a city
2. convert_currency(amount: string, from: string, to: string) — converts an amount from one currency to another

Given the user's request, respond ONLY with JSON in this exact format:
{"toolName": "<tool name>", "arguments": {"<param>": "<value>", ...}}

Do not include any other text, explanation, or markdown formatting.
"""

func callLLM(userRequest: String) async throws -> ToolCall {
    guard let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
        throw NSError(domain: "ToolCalling", code: 0, userInfo: [NSLocalizedDescriptionKey: "GROQ_API_KEY not set in environment"])
    }
    
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    
    let body: [String: Any] = [
            "model": "openai/gpt-oss-120b",
            "temperature": 0.2,
            "messages": [
                ["role": "system", "content": toolDescriptions],
                ["role": "user", "content": userRequest]
            ]
        ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)
    let (data, _) = try await URLSession.shared.data(for: request)
    
    struct GroqResponse: Codable {
            struct Choice: Codable {
                struct Message: Codable {
                    let content: String
                }
                let message: Message
            }
            let choices: [Choice]
        }
    let groqResponse = try JSONDecoder().decode(GroqResponse.self, from: data)
    let rawContent = groqResponse.choices[0].message.content
    
    guard let contentData = rawContent.data(using: .utf8) else {
           throw NSError(domain: "ToolCalling", code: 1, userInfo: [NSLocalizedDescriptionKey: "Could not convert response to data"])
       }
    return try JSONDecoder().decode(ToolCall.self, from: contentData)
}

func getWeather(city: String) -> String {
    return "It's sunny and 28°C in \(city)."
}

func convertCurrency(amount: String, from: String, to: String) -> String {
    return "\(amount) \(from) is approximately \(amount) \(to) (mock conversion, no real rate applied)."
}

func dispatch(toolCall: ToolCall) -> String {
    switch toolCall.toolName {
    case "get_weather":
        guard let city = toolCall.arguments["city"] else {
            return "Error: missing 'city' argument"
        }
        return getWeather(city: city)

    case "convert_currency":
        guard let amount = toolCall.arguments["amount"],
              let from = toolCall.arguments["from"],
              let to = toolCall.arguments["to"] else {
            return "Error: missing currency arguments"
        }
        return convertCurrency(amount: amount, from: from, to: to)

    default:
        return "Error: unknown tool '\(toolCall.toolName)'"
    }
}

do {
    //let userRequest = "What's the weather like in Bangalore?"
   // let userRequest = "Convert 100 USD to EUR"
    let userRequest = "Hello, how are you?"
    let toolCall = try await callLLM(userRequest: userRequest)
    print("LLM chose tool: \(toolCall.toolName), arguments: \(toolCall.arguments)")

    let result = dispatch(toolCall: toolCall)
    print("Result: \(result)")
} catch {
    print("Error: \(error)")
}

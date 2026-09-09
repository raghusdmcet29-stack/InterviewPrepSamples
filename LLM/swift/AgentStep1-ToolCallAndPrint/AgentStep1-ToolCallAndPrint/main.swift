//
//  main.swift
//  AgentStep1-ToolCallAndPrint
//
//  Created by Anussha on 07/09/26.
//

import Foundation

// MARK: - Mock tool
func getWeather(city: String) -> String {
    return "The weather in \(city) is 24°C and sunny."
}

// MARK: - Tool schema sent to the model
let tools: [[String: Any]] = [
    [
        "type": "function",
        "function": [
            "name": "get_weather",
            "description": "Get current weather for a city",
            "parameters": [
                "type": "object",
                "properties": [
                    "city": ["type": "string", "description": "City name"]
                ],
                "required": ["city"]
            ]
        ]
    ]
]

// MARK: - Call the model once, ask it to pick a tool
func askModelForToolCall(userMessage: String) async throws -> (id: String, name: String, arguments: [String: Any])? {
    let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] ?? ""
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!

    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "messages": [["role": "user", "content": userMessage]],
        "tools": tools
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (data, _) = try await URLSession.shared.data(for: request)
    //print("RAW RESPONSE: \(String(data: data, encoding: .utf8) ?? "unreadable")")   // <-- ADD THIS LINE
    let json = try JSONSerialization.jsonObject(with: data) as? [String: Any]

    guard
           let choices = json?["choices"] as? [[String: Any]],
           let message = choices.first?["message"] as? [String: Any],
           let toolCalls = message["tool_calls"] as? [[String: Any]],
           let firstCall = toolCalls.first,
           let callId = firstCall["id"] as? String,          // NEW
           let function = firstCall["function"] as? [String: Any],
           let name = function["name"] as? String,
           let argsString = function["arguments"] as? String,
           let argsData = argsString.data(using: .utf8),
           let arguments = try JSONSerialization.jsonObject(with: argsData) as? [String: Any]
       else {
           return nil
       }

       return (callId, name, arguments)
}


// MARK: - Call the model a second time with the tool result, to get a final natural-language answer
func askModelForFinalAnswer(userMessage: String, call: (id: String, name: String, arguments: [String: Any]), toolResult: String) async throws -> String? {
    let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] ?? ""
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!

    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")

    // Re-encode the original arguments back into a JSON string, since that's the format
    // the "assistant" tool_calls message expects (matches what the model originally sent)
    let argsData = try JSONSerialization.data(withJSONObject: call.arguments)
    let argsString = String(data: argsData, encoding: .utf8) ?? "{}"

    let messages: [[String: Any]] = [
        ["role": "user", "content": userMessage],
        ["role": "assistant", "content": NSNull(), "tool_calls": [
            [
                "id": call.id,
                "type": "function",
                "function": ["name": call.name, "arguments": argsString]
            ]
        ]],
        ["role": "tool", "tool_call_id": call.id, "content": toolResult]
    ]

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "messages": messages
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (data, _) = try await URLSession.shared.data(for: request)
    let json = try JSONSerialization.jsonObject(with: data) as? [String: Any]

    guard
        let choices = json?["choices"] as? [[String: Any]],
        let message = choices.first?["message"] as? [String: Any],
        let content = message["content"] as? String
    else {
        return nil
    }

    return content
}



Task {
    let userMessage = "What's the weather in Tokyo?"

    if let call = try await askModelForToolCall(userMessage: userMessage) {
        print("Model wants to call: \(call.name) with args: \(call.arguments)")

        if call.name == "get_weather", let city = call.arguments["city"] as? String {
            let result = getWeather(city: city)
            print("Raw tool result: \(result)")

            // NEW — send the result back to the model for a final natural-language answer
            if let finalAnswer = try await askModelForFinalAnswer(userMessage: userMessage, call: call, toolResult: result) {
                print("Final answer: \(finalAnswer)")
            } else {
                print("Model did not return a final answer.")
            }
        }
    } else {
        print("Model did not return a tool call.")
    }
}

RunLoop.main.run()

RunLoop.main.run()

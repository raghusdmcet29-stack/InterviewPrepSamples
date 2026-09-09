//
//  main.swift
//  AgentStep1-ToolCallAndPrint
//
//  Created by Anussha on 07/09/26.
//

import Foundation

// MARK: - Mock tools
func getWeather(city: String) -> String {
    return "The weather in \(city) is 24°C and sunny."
}

func lookupCompanyHQ(company: String) -> String {
    let hqs = ["Acme Corp": "Tokyo", "Globex": "London"]
    return hqs[company] ?? "Unknown"
}

// MARK: - Tool schemas sent to the model
let weatherTool: [String: Any] = [
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

let lookupHQTool: [String: Any] = [
    "type": "function",
    "function": [
        "name": "lookup_company_hq",
        "description": "Look up the city where a company's headquarters is located",
        "parameters": [
            "type": "object",
            "properties": [
                "company": ["type": "string", "description": "The company name"]
            ],
            "required": ["company"]
        ]
    ]
]

let allTools: [[String: Any]] = [weatherTool, lookupHQTool]

// MARK: - One generic call to the model, given the full message history so far.
// Returns the raw "message" dict from the API response (could contain tool_calls OR content).
func callModel(messages: [[String: Any]]) async throws -> [String: Any]? {
    let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] ?? ""
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!

    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "messages": messages,
        "tools": allTools
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (data, _) = try await URLSession.shared.data(for: request)
    let json = try JSONSerialization.jsonObject(with: data) as? [String: Any]

    guard
        let choices = json?["choices"] as? [[String: Any]],
        let message = choices.first?["message"] as? [String: Any]
    else {
        return nil
    }

    return message
}

// MARK: - Pull a tool call (id, name, arguments) out of a message dict, if present
func extractToolCall(from message: [String: Any]) -> (id: String, name: String, arguments: [String: Any])? {
    guard
        let toolCalls = message["tool_calls"] as? [[String: Any]],
        let firstCall = toolCalls.first,
        let callId = firstCall["id"] as? String,
        let function = firstCall["function"] as? [String: Any],
        let name = function["name"] as? String,
        let argsString = function["arguments"] as? String,
        let argsData = argsString.data(using: .utf8),
        let arguments = try? JSONSerialization.jsonObject(with: argsData) as? [String: Any]
    else {
        return nil
    }
    return (callId, name, arguments)
}

// MARK: - Build the "assistant" message dict that records the tool call, for appending to history
func assistantToolCallMessage(id: String, name: String, arguments: [String: Any]) -> [String: Any] {
    let argsData = (try? JSONSerialization.data(withJSONObject: arguments)) ?? Data()
    let argsString = String(data: argsData, encoding: .utf8) ?? "{}"

    return [
        "role": "assistant",
        "content": NSNull(),
        "tool_calls": [
            [
                "id": id,
                "type": "function",
                "function": ["name": name, "arguments": argsString]
            ]
        ]
    ]
}

// MARK: - Run the mock function matching a tool name
func executeTool(name: String, arguments: [String: Any]) -> String {
    switch name {
    case "get_weather":
        let city = arguments["city"] as? String ?? "Unknown"
        return getWeather(city: city)
    case "lookup_company_hq":
        let company = arguments["company"] as? String ?? "Unknown"
        return lookupCompanyHQ(company: company)
    default:
        return "Unknown tool: \(name)"
    }
}

// MARK: - The chaining loop
Task {
    var messages: [[String: Any]] = [
        ["role": "user", "content": "What's the weather at Acme Corp's headquarters?"]
    ]

    let maxTurns = 5
    var turn = 0

    while turn < maxTurns {
        turn += 1

        guard let message = try await callModel(messages: messages) else {
            print("Turn \(turn): no response from model.")
            break
        }

        if let call = extractToolCall(from: message) {
            print("Turn \(turn): model wants to call \(call.name) with \(call.arguments)")

            // record the assistant's tool-call message in history
            messages.append(assistantToolCallMessage(id: call.id, name: call.name, arguments: call.arguments))

            // actually run the mock tool
            let result = executeTool(name: call.name, arguments: call.arguments)
            print("Turn \(turn): tool result = \(result)")

            // record the tool's result in history
            messages.append([
                "role": "tool",
                "tool_call_id": call.id,
                "content": result
            ])

            // loop again — model sees the updated history and decides what to do next

        } else if let content = message["content"] as? String {
            print("Final answer: \(content)")
            break
        } else {
            print("Turn \(turn): unexpected response shape, stopping.")
            break
        }
    }
}

RunLoop.main.run()

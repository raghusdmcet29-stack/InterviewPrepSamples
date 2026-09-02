//
//  main.swift
//  NativeFunctionCallingCLI
//
//  Created by Anussha on 02/09/26.
//

import Foundation

let tools: [[String: Any]] = [
    [
        "type": "function",
        "function": [
            "name": "get_weather",
            "description": "Returns current weather for a city",
            "parameters": [
                "type": "object",
                "properties": [
                    "city": ["type": "string", "description": "The city name"]
                ],
                "required": ["city"]
            ]
        ]
    ],
    [
        "type": "function",
        "function": [
            "name": "convert_currency",
            "description": "Converts an amount from one currency to another",
            "parameters": [
                "type": "object",
                "properties": [
                    "amount": ["type": "string"],
                    "from": ["type": "string"],
                    "to": ["type": "string"]
                ],
                "required": ["amount", "from", "to"]
            ]
        ]
    ]
]



struct GroqToolResponse: Codable {
    struct Choice: Codable {
        struct Message: Codable {
            struct ToolCallItem: Codable {
                struct Function: Codable {
                    let name: String
                    let arguments: String  // still a JSON string — one more decode step needed
                }
                let function: Function
            }
            let tool_calls: [ToolCallItem]?
        }
        let message: Message
    }
    let choices: [Choice]
}

func callLLMNative(userRequest: String) async throws -> GroqToolResponse.Choice.Message.ToolCallItem {
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
            ["role": "user", "content": userRequest]
        ],
        "tools": tools,
        "tool_choice": "auto"
    ]

    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (data, _) = try await URLSession.shared.data(for: request)
    let decoded = try JSONDecoder().decode(GroqToolResponse.self, from: data)

    guard let toolCall = decoded.choices.first?.message.tool_calls?.first else {
        throw NSError(domain: "ToolCalling", code: 2, userInfo: [NSLocalizedDescriptionKey: "Model did not return a tool call"])
    }

    return toolCall
}

struct WeatherArgs: Codable {
    let city: String
}

struct CurrencyArgs: Codable {
    let amount: String
    let from: String
    let to: String
}

func getWeather(city: String) -> String {
    return "It's sunny and 28°C in \(city)."
}

func convertCurrency(amount: String, from: String, to: String) -> String {
    return "\(amount) \(from) is approximately \(amount) \(to) (mock conversion, no real rate applied)."
}

func dispatchNative(_ toolCall: GroqToolResponse.Choice.Message.ToolCallItem) throws -> String {
    guard let argsData = toolCall.function.arguments.data(using: .utf8) else {
        throw NSError(domain: "ToolCalling", code: 3, userInfo: [NSLocalizedDescriptionKey: "Could not convert arguments to data"])
    }

    switch toolCall.function.name {
    case "get_weather":
        let args = try JSONDecoder().decode(WeatherArgs.self, from: argsData)
        return getWeather(city: args.city)

    case "convert_currency":
        let args = try JSONDecoder().decode(CurrencyArgs.self, from: argsData)
        return convertCurrency(amount: args.amount, from: args.from, to: args.to)

    default:
        return "Error: unknown tool '\(toolCall.function.name)'"
    }
}

//let userRequest = "What's the weather like in Bangalore?"
let userRequest = "Hello, how are you?"
do {
    let toolCall = try await callLLMNative(userRequest: userRequest)
    print("LLM chose tool: \(toolCall.function.name), raw arguments: \(toolCall.function.arguments)")

    let result = try dispatchNative(toolCall)
    print("Result: \(result)")
} catch {
    print("Error: \(error)")
}


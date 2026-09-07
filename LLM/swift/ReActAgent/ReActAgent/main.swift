//
//  main.swift
//  ReActAgent
//
//  Created by Anussha on 04/09/26.
//

import Foundation

// Mock "tools" - in production these would call real APIs
func lookupCompanyHQ(company: String) -> String {
    // hardcoded mock - in production this would query a real database/API
    if company.lowercased().contains("groq") {
        return "Mountain View, California"
    }
    return "Unknown"
}

func getWeather(city: String) -> String {
    // hardcoded mock
    if city.contains("Mountain View") {
        return "68°F, Sunny"
    }
    return "Weather data unavailable"
}

// Tool schema definitions - sent to the model so it knows what it can call
let toolsSchema: [[String: Any]] = [
    [
        "type": "function",
        "function": [
            "name": "lookup_company_hq",
            "description": "Look up the headquarters city and state/country of a company",
            "parameters": [
                "type": "object",
                "properties": [
                    "company": ["type": "string", "description": "The company name"]
                ],
                "required": ["company"]
            ]
        ]
    ],
    [
        "type": "function",
        "function": [
            "name": "get_weather",
            "description": "Get the current weather for a city",
            "parameters": [
                "type": "object",
                "properties": [
                    "city": ["type": "string", "description": "The city name"]
                ],
                "required": ["city"]
            ]
        ]
    ]
]

// A single message in the conversation - role can be "user", "assistant", "tool", or "system"
struct Message {
    let role: String
    let content: String?
    let toolCallId: String?   // set only on "tool" role messages - links the result back to the specific call
    let toolCalls: [[String: Any]]?   // set only on "assistant" messages that requested tool calls
    
    // Converts this message into the dictionary shape the Groq API expects
    func toDict() -> [String: Any] {
        var dict: [String: Any] = ["role": role]
        if let content = content { dict["content"] = content }
        if let toolCallId = toolCallId { dict["tool_call_id"] = toolCallId }
        if let toolCalls = toolCalls { dict["tool_calls"] = toolCalls }
        return dict
    }
}

func callGroq(messages: [Message]) throws -> [String: Any] {
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
        "messages": messages.map { $0.toDict() },
        "tools": toolsSchema,
        "temperature": 0.2
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)
    
    let semaphore = DispatchSemaphore(value: 0)
    var result: [String: Any] = [:]
    
    URLSession.shared.dataTask(with: request) { data, _, _ in
        if let data = data,
           let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any] {
            result = json
        }
        semaphore.signal()
    }.resume()
    
    semaphore.wait()
    return result
}

func runAgent(userQuestion: String) {
    var messages: [Message] = [
        Message(role: "user", content: userQuestion, toolCallId: nil, toolCalls: nil)
    ]
    
    let maxTurns = 5   // safety guard against infinite tool-calling loops
    var turn = 0
    
    while turn < maxTurns {
        turn += 1
        print("--- Turn \(turn) ---")
        
        guard let response = try? callGroq(messages: messages),
              let choices = response["choices"] as? [[String: Any]],
              let firstChoice = choices.first,
              let message = firstChoice["message"] as? [String: Any] else {
            print("Error: malformed response")
            return
        }
        
        // Case 1: model wants to call a tool
        if let toolCalls = message["tool_calls"] as? [[String: Any]] {
            // Record the assistant's tool-call request in history
            messages.append(Message(role: "assistant", content: nil, toolCallId: nil, toolCalls: toolCalls))
            
            for toolCall in toolCalls {
                guard let id = toolCall["id"] as? String,
                      let function = toolCall["function"] as? [String: Any],
                      let name = function["name"] as? String,
                      let argsString = function["arguments"] as? String,
                      let argsData = argsString.data(using: .utf8),
                      let args = try? JSONSerialization.jsonObject(with: argsData) as? [String: Any] else {
                    print("Error: malformed tool call")
                    return
                }
                
                let toolResult: String
                if name == "lookup_company_hq", let company = args["company"] as? String {
                    toolResult = lookupCompanyHQ(company: company)
                    print("Called lookup_company_hq(\(company)) -> \(toolResult)")
                } else if name == "get_weather", let city = args["city"] as? String {
                    toolResult = getWeather(city: city)
                    print("Called get_weather(\(city)) -> \(toolResult)")
                } else {
                    toolResult = "Unknown tool"
                }
                
                // Feed the result back into the conversation as a "tool" message
                messages.append(Message(role: "tool", content: toolResult, toolCallId: id, toolCalls: nil))
            }
            // Loop continues - model will see the tool result on the next turn
            
        // Case 2: model gave a final plain-text answer - terminate the loop
        } else if let content = message["content"] as? String {
            print("Final answer: \(content)")
            return
        } else {
            print("Error: response had neither tool_calls nor content")
            return
        }
    }
    
    print("Max turns reached without a final answer")
}
runAgent(userQuestion: "What's the weather in the city where Groq is headquartered?")


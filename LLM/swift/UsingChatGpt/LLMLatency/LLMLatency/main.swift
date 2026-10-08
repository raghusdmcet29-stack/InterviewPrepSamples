//
//  main.swift
//  LLMLatency
//
//  Created by Anussha on 08/10/26.
//

import Foundation

func callGroq() async -> String? {

    guard let key = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
        print("ERROR: GROQ_API_KEY is not set")
        return nil
    }

    let url = URL(
        string: "https://api.groq.com/openai/v1/chat/completions"
    )!

    var request = URLRequest(url: url)
    request.httpMethod = "POST"

    request.setValue(
        "Bearer \(key)",
        forHTTPHeaderField: "Authorization"
    )

    request.setValue(
        "application/json",
        forHTTPHeaderField: "Content-Type"
    )

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "messages": [
            [
                "role": "user",
                "content": "What is the capital of France?"
            ]
        ]
    ]

    do {

        request.httpBody = try JSONSerialization.data(
            withJSONObject: body
        )

        let (data, response) = try await URLSession.shared.data(
            for: request
        )

        if let httpResponse = response as? HTTPURLResponse {
            print("HTTP status:", httpResponse.statusCode)
        }

        let json = try JSONSerialization.jsonObject(
            with: data
        ) as! [String: Any]

        let choices = json["choices"] as! [[String: Any]]
        let message = choices[0]["message"] as! [String: Any]
        let answer = message["content"] as! String

        return answer

    } catch {
        print("ERROR:", error.localizedDescription)
        return nil
    }
}

print("LLM Latency Test")
print("================")

let start = Date()

let answer = await callGroq()

let elapsed = Date().timeIntervalSince(start)

if let answer = answer {
    print("")
    print("LLM Answer:")
    print(answer)
}

print("")
print("Elapsed time: \(String(format: "%.3f", elapsed)) seconds")


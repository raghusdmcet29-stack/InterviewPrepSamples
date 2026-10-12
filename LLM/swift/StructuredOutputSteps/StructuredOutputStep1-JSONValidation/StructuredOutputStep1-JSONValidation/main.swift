//
//  main.swift
//  StructuredOutputStep1-JSONValidation
//
//  Created by Anussha on 09/10/26.
//

import Foundation

func isValidJSON(_ reply: String) -> Bool {
    guard let data = reply.data(using: .utf8) else { return false }
    return (try? JSONSerialization.jsonObject(with: data)) != nil
}

func hasRequiredFields(_ reply: String) -> Bool {
    guard let data = reply.data(using: .utf8),
          let object = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
        return false
    }
    return object["name"] != nil && object["age"] != nil
}

func isAccepted(_ reply: String) -> Bool {
    return isValidJSON(reply) && hasRequiredFields(reply)
}

func callGroq(question: String, prompt: String) async -> String {
    guard let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
        return "ERROR: GROQ_API_KEY not set"
    }
    guard let url = URL(string: "https://api.groq.com/openai/v1/chat/completions") else {
        return "ERROR: bad url"
    }
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "temperature": 0,
        "messages": [
            ["role": "system", "content": prompt],
            ["role": "user", "content": question]
        ]
    ]
    do {
        request.httpBody = try JSONSerialization.data(withJSONObject: body)
        let (data, _) = try await URLSession.shared.data(for: request)
        if let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
           let choices = json["choices"] as? [[String: Any]],
           let message = choices.first?["message"] as? [String: Any],
           let content = message["content"] as? String {
            return content
        }
        return "ERROR: unexpected response"
    } catch {
        return "ERROR: \(error)"
    }
}
func askForPerson(maxTries: Int) async -> String? {
    for attempt in 1...maxTries {
        let prompt = attempt == 1
            ? "Reply in one friendly sentence."
            : "Your last reply was not valid JSON. Reply only with valid JSON with keys name and age, and nothing else."
        let reply = await callGroq(
            question: "Give me a person's name and age.",
            prompt: prompt
        )
        print("attempt \(attempt): \(reply)")
        if isAccepted(reply) {
            return reply
        }
    }
    return nil
}

/*func askForPerson(maxTries: Int) async -> String? {
    for attempt in 1...maxTries {
        let reply = await callGroq(
            question: "Give me a person's name and age.",
            prompt: "Reply only with valid JSON with keys name and age, and nothing else."
        )
        print("attempt \(attempt): \(reply)")
        if isAccepted(reply) {
            return reply
        }
    }
    return nil
}
*/
let result = await askForPerson(maxTries: 3)
print("final:", result ?? "none after 3 tries")



//
//  main.swift
//  StructuredOutputStep7-CodeFenceCleaning
//
//  Created by Anussha on 13/10/26.
//

import Foundation

struct Person: Codable {
    let name: String
    let age: Int
}

struct Rejection: Error {
    let reason: String
}

struct GroqResponse: Codable {
    struct Choice: Codable {
        struct Message: Codable {
            let content: String
        }
        let message: Message
    }
    let choices: [Choice]
}

let allowedKeys: Set<String> = ["name", "age"]

func cleanFence(_ text: String) -> String {
    var lines = text.components(separatedBy: "\n")
    guard let first = lines.first, first.hasPrefix("```") else {
        return text
    }
    lines.removeFirst()
    if let last = lines.last, last.hasPrefix("```") {
        lines.removeLast()
    }
    return lines.joined(separator: "\n").trimmingCharacters(in: .whitespacesAndNewlines)
}

func validate(_ json: String) -> Result<Person, Rejection> {
    guard let data = json.data(using: .utf8) else {
        return .failure(Rejection(reason: "bad encoding"))
    }
    guard let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
        return .failure(Rejection(reason: "not valid JSON"))
    }
    if Set(obj.keys) != allowedKeys {
        return .failure(Rejection(reason: "unexpected or missing fields"))
    }
    guard let person = try? JSONDecoder().decode(Person.self, from: data) else {
        return .failure(Rejection(reason: "wrong types for name or age"))
    }
    if person.name.trimmingCharacters(in: .whitespaces).isEmpty {
        return .failure(Rejection(reason: "empty name"))
    }
    if person.age < 0 || person.age > 120 {
        return .failure(Rejection(reason: "age out of range"))
    }
    return .success(person)
}

let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] ?? ""

func askModel(_ prompt: String) async throws -> String {
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!

    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "temperature": 0,
        "max_tokens": 200,
        "messages": [["role": "user", "content": prompt]]
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (data, _) = try await URLSession.shared.data(for: request)
    let decoded = try JSONDecoder().decode(GroqResponse.self, from: data)
    return decoded.choices[0].message.content
}

func getValidPerson(question: String, maxTries: Int) async -> Person? {
    var prompt = question
    for i in 1...maxTries {
        let reply: String
        do {
            reply = try await askModel(prompt)
        } catch {
            print("Attempt \(i): API error, \(error)")
            continue
        }
        print("Attempt \(i) reply: \(reply)")
        switch validate(cleanFence(reply)) {
        case .success(let person):
            print("Attempt \(i): accepted")
            return person
        case .failure(let rejection):
            print("Attempt \(i): rejected, \(rejection.reason)")
            prompt = question + " Your last reply was rejected because: \(rejection.reason). Fix it and reply with JSON only, with exactly the fields name and age. The age must be a number, not text, for example 25."
        }
    }
    return nil
}

Task {
    if apiKey.isEmpty {
        print("GROQ_API_KEY is not set")
    } else if let person = await getValidPerson(question: "Give a name and the age written as a word, as JSON only, like {\"name\": \"...\", \"age\": \"...\"}.", maxTries: 3) {
        print("Final: \(person.name), \(person.age)")
    } else {
        print("No valid reply after 3 attempts")
    }
    exit(0)
}

RunLoop.main.run()



/*
import Foundation

func cleanFence(_ text: String) -> String {
    var lines = text.components(separatedBy: "\n")
    guard let first = lines.first, first.hasPrefix("```") else {
        return text
    }
    lines.removeFirst()
    if let last = lines.last, last.hasPrefix("```") {
        lines.removeLast()
    }
    return lines.joined(separator: "\n").trimmingCharacters(in: .whitespacesAndNewlines)
}

print(cleanFence("{\"name\": \"Ravi\", \"age\": 34}"))
print(cleanFence("```json\n{\"name\": \"Ravi\", \"age\": 34}\n```"))
*/

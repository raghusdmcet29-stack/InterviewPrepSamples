//
//  main.swift
//  EvalStep1-ContainsScoring
//
//  Created by Anussha on 28/09/26.
//

import Foundation

struct TestCase {
    let question: String
    let expected: String
}

let testCases = [
    TestCase(question: "What is the capital of France?", expected: "Paris"),
    TestCase(question: "What is 7 times 8?", expected: "56"),
    TestCase(question: "What is the capital of Japan?", expected: "Tokyo")
]

for testCase in testCases {
    print("Q: \(testCase.question)  |  Expected: \(testCase.expected)")
}
func containsScore(reply: String, expected: String) -> Bool {
    return reply.lowercased().contains(expected.lowercased())
}

func callGroq(question: String) async -> String {
    guard let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
        return "ERROR: GROQ_API_KEY not set"
    }
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "temperature": 0,
        "messages": [["role": "user", "content": question]]
    ]
    request.httpBody = try? JSONSerialization.data(withJSONObject: body)
    do {
        let (data, _) = try await URLSession.shared.data(for: request)
        let json = try JSONSerialization.jsonObject(with: data) as? [String: Any]
        let choices = json?["choices"] as? [[String: Any]]
        let message = choices?.first?["message"] as? [String: Any]
        return message?["content"] as? String ?? "ERROR: unexpected response"
    } catch {
        return "ERROR: \(error)"
    }
}

print("---- running eval ----")
var passed = 0

for testCase in testCases {
    let reply = await callGroq(question: testCase.question)
    let ok = containsScore(reply: reply, expected: testCase.expected)
    if ok { passed += 1 }
    print("\(ok ? "PASS" : "FAIL") | \(testCase.question) | got: \(reply.trimmingCharacters(in: .whitespacesAndNewlines))")
}

print("Score: \(passed)/\(testCases.count)")
/*
print("---- testing containsScore ----")
print(containsScore(reply: "The capital is Paris.", expected: "Paris"))
print(containsScore(reply: "paris", expected: "Paris"))
print(containsScore(reply: "Lyon", expected: "Paris"))
print("---- testing callGroq ----")
print(await callGroq(question: "What is the capital of France?"))
*/

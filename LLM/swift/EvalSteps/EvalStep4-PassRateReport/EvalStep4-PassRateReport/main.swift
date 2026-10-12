//
//  main.swift
//  EvalStep4-PassRateReport
//
//  Created by Anussha on 01/10/26.
//

import Foundation

func passCount(scores: [Int]) -> Int {
    return scores.reduce(0, +)
}
func wholeWordScore(response: String, expected: String) -> Int {
    let escaped = NSRegularExpression.escapedPattern(for: expected)
    let pattern = "(?<!\\d)" + escaped + "(?!\\d)"
    if response.range(of: pattern, options: [.regularExpression, .caseInsensitive]) != nil {
        return 1
    }
    return 0
}


func passRateReport(scores: [Int]) -> String {
    let passes = passCount(scores: scores)
    let total = scores.count
    guard total > 0 else { return "No test cases to score" }
    let percent = Double(passes) / Double(total) * 100
    return "\(passes)/\(total) = \(String(format: "%.1f", percent))%"
}

struct TestCase {
    let question: String
    let expected: String
}

let testCases: [TestCase] = [
    TestCase(question: "What is the capital of France?", expected: "Paris"),
    TestCase(question: "What is 7 times 8?", expected: "56"),
    TestCase(question: "What is the capital of Japan?", expected: "Tokyo"),
    TestCase(question: "What is 9 times 9?", expected: "81"),
    TestCase(question: "What is the capital of Italy?", expected: "Rome"),
    TestCase(question: "What is 12 plus 15?", expected: "27"),
    TestCase(question: "What is the capital of Spain?", expected: "Madrid"),
    TestCase(question: "What is 100 minus 37?", expected: "63"),
    TestCase(question: "What is the capital of Egypt?", expected: "Cairo"),
    TestCase(question: "What is 6 times 7?", expected: "42")
]

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

var scores: [Int] = []

for tc in testCases {
    let answer = await callGroq(question: tc.question)
    let score = wholeWordScore(response: answer, expected: tc.expected)
    scores.append(score)
    print("\(score == 1 ? "PASS" : "FAIL") | \(tc.question) -> \(answer)")
}

print(passRateReport(scores: scores))
/*
print(passCount(scores: [1, 1, 0, 1]))
print(passCount(scores: [1, 0, 1, 1, 0]))
print(passCount(scores: [0, 0, 0]))
print(passRateReport(scores: [1, 1, 0, 1]))
print(passRateReport(scores: [1, 0, 0]))
print(passRateReport(scores: []))
print("Test cases: \(testCases.count)")

*/
/*
let first = testCases[0]
let answer = await callGroq(question: first.question)
let score = wholeWordScore(response: answer, expected: first.expected)

print("Q: \(first.question)")
print("A: \(answer)")
print("Score: \(score)")
*/

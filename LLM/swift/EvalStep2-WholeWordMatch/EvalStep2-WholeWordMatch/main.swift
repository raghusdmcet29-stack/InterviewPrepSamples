//
//  main.swift
//  EvalStep2-WholeWordMatch
//
//  Created by Anussha on 29/09/26.
//

import Foundation

func wholeWordScore(response: String, expected: String) -> Int {
    let escaped = NSRegularExpression.escapedPattern(for: expected)
    let pattern = "(?<!\\d)" + escaped + "(?!\\d)"
    if response.range(of: pattern, options: [.regularExpression, .caseInsensitive]) != nil {
        return 1
    }
    return 0
}
/*
print(wholeWordScore(response: "The answer is 56.", expected: "56"))
print(wholeWordScore(response: "156", expected: "56"))
print(wholeWordScore(response: "[56]", expected: "56"))
print(wholeWordScore(response: "5566", expected: "56"))*/

struct TestCase {
    let question: String
    let expected: String
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

let testCases = [
    TestCase(question: "What is the capital of France?", expected: "Paris"),
    TestCase(question: "What is 7 times 8?", expected: "56"),
    TestCase(question: "What is the capital of Japan?", expected: "Tokyo")
]

var total = 0
for testCase in testCases {
    let answer = await callGroq(question: testCase.question)
    let score = wholeWordScore(response: answer, expected: testCase.expected)
    total += score
    print(score == 1 ? "PASS" : "FAIL", "|", testCase.question, "|", answer)
}
print("Score:", total, "/", testCases.count)


/*
for testCase in testCases {
    print("Q:", testCase.question, "| expected:", testCase.expected)
}

let answer = await callGroq(question: "What is 7 times 8?")
print("Model said:", answer)

*/


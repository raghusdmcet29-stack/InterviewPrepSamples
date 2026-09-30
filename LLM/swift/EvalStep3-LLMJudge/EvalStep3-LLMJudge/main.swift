//
//  main.swift
//  EvalStep3-LLMJudge
//
//  Created by Anussha on 30/09/26.
//

import Foundation

func buildJudgePrompt(question: String, expected: String, answer: String) -> String {
    return """
    You are grading an answer to a question.
    Decide if the answer means the same as the expected answer.
    Reply with only one word: PASS or FAIL.

    Question: \(question)
    Expected answer: \(expected)
    Answer to grade: \(answer)
    """
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

func judgeScore(question: String, expected: String, answer: String) async -> Int {
    let prompt = buildJudgePrompt(question: question, expected: expected, answer: answer)
    let reply = await callGroq(question: prompt)
    let cleaned = reply.trimmingCharacters(in: .whitespacesAndNewlines).uppercased()
    return cleaned == "PASS" ? 1 : 0
}

print("A:", await judgeScore(question: "What is 7 x 8?", expected: "56", answer: "Fifty-six."))
print("B:", await judgeScore(question: "What is the capital of Japan?", expected: "Tokyo", answer: "Kyoto."))
print("C:", await judgeScore(question: "What is 7 x 8?", expected: "56", answer: "The result is 156."))

struct TestCase {
    let question: String
    let expected: String
}

let testCases = [
    TestCase(question: "What is the capital of France?", expected: "Paris"),
    TestCase(question: "What is 7 x 8?", expected: "56"),
    TestCase(question: "What is the capital of Japan?", expected: "Tokyo")
]

var passed = 0
for testCase in testCases {
    let answer = await callGroq(question: testCase.question)
    let score = await judgeScore(question: testCase.question, expected: testCase.expected, answer: answer)
    passed += score
    print(score == 1 ? "PASS" : "FAIL", "|", testCase.question, "|", answer)
}
print("Score: \(passed)/\(testCases.count)")


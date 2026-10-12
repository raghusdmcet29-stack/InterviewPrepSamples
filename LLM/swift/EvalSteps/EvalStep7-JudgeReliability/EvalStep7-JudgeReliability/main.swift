//
//  main.swift
//  EvalStep7-JudgeReliability
//
//  Created by Anussha on 07/10/26.
//

import Foundation

struct JudgeCase {
    let question: String
    let answer: String
    let label: String   // our known verdict: "PASS" or "FAIL"
}

let cases = [
    JudgeCase(question: "What is the capital of France?", answer: "Paris", label: "PASS"),
    JudgeCase(question: "What is the capital of France?", answer: "Lyon", label: "FAIL"),
    JudgeCase(question: "What is 7 times 8?", answer: "Fifty-six", label: "PASS"),
    JudgeCase(question: "What is 7 times 8?", answer: "54", label: "FAIL"),
    JudgeCase(question: "What is the capital of France?", answer: "Paris is the capital of Germany", label: "FAIL"),
    JudgeCase(question: "What is the capital of Australia?", answer: "Sydney", label: "FAIL"),
    JudgeCase(question: "What is the capital of Australia?", answer: "Canberra, not Sydney", label: "PASS"),
    JudgeCase(question: "What is 7 times 8?", answer: "56.0", label: "PASS"),
]

let runsPerCase = 5

func callGroqOnce(prompt: String) async throws -> String {
    guard let key = ProcessInfo.processInfo.environment["GROQ_API_KEY"] else {
        throw NSError(domain: "Judge", code: 1,
                      userInfo: [NSLocalizedDescriptionKey: "GROQ_API_KEY not set"])
    }
    var request = URLRequest(url: URL(string: "https://api.groq.com/openai/v1/chat/completions")!)
    request.httpMethod = "POST"
    request.setValue("Bearer \(key)", forHTTPHeaderField: "Authorization")
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "temperature": 0,
        "messages": [["role": "user", "content": prompt]]
    ]
    request.httpBody = try JSONSerialization.data(withJSONObject: body)

    let (data, _) = try await URLSession.shared.data(for: request)
    guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
          let choices = json["choices"] as? [[String: Any]],
          let message = choices.first?["message"] as? [String: Any],
          let content = message["content"] as? String else {
        throw NSError(domain: "Judge", code: 2,
                      userInfo: [NSLocalizedDescriptionKey: "Unexpected response"])
    }
    return content
}

func callGroq(prompt: String) async -> String? {
    for attempt in 1...3 {
        do {
            return try await callGroqOnce(prompt: prompt)
        } catch {
            print("API error (try \(attempt)): \(error.localizedDescription)")
            if attempt < 3 {
                try? await Task.sleep(nanoseconds: UInt64(attempt) * 5_000_000_000)
            }
        }
    }
    return nil
}

func buildJudgePrompt(question: String, answer: String) -> String {
    return "Question: \(question)\nAnswer: \(answer)\nIs the answer correct? Reply with exactly one word: PASS or FAIL."
}


// Returns "PASS", "FAIL", "OTHER" (unexpected reply), or nil (API failed)
func judgeVerdict(question: String, answer: String) async -> String? {
    guard let reply = await callGroq(prompt: buildJudgePrompt(question: question, answer: answer)) else {
        return nil
    }
    let cleaned = reply.trimmingCharacters(in: .whitespacesAndNewlines).uppercased()
    if cleaned == "PASS" || cleaned == "FAIL" {
        return cleaned
    }
    return "OTHER"
}

var totalMatches = 0
var totalVerdicts = 0
var falsePasses = 0
var consistencies: [Double] = []

for (index, testCase) in cases.enumerated() {
    var verdicts: [String] = []
    for _ in 0..<runsPerCase {
        if let verdict = await judgeVerdict(question: testCase.question, answer: testCase.answer) {
            verdicts.append(verdict)
        }
        try? await Task.sleep(nanoseconds: 2_000_000_000)
    }
   
    guard !verdicts.isEmpty else {
        print("Case \(index + 1): no verdicts (API errors)")
        continue
    }

    let counts = Dictionary(grouping: verdicts, by: { $0 }).mapValues { $0.count }
    let top = counts.values.max() ?? 0
    let consistency = Double(top) / Double(verdicts.count) * 100
    consistencies.append(consistency)

    let matches = verdicts.filter { $0 == testCase.label }.count
    totalMatches += matches
    totalVerdicts += verdicts.count
    if testCase.label == "FAIL" {
        falsePasses += verdicts.filter { $0 == "PASS" }.count
    }

    print("Case \(index + 1): \(testCase.question) | \"\(testCase.answer)\" | label \(testCase.label)")
    print("  verdicts: \(verdicts.joined(separator: " "))")
    print("  consistency \(top)/\(verdicts.count) = \(String(format: "%.1f", consistency))%")
}
print(totalVerdicts)
guard totalVerdicts > 0 else {
    print("No verdicts collected")
    exit(1)
}
print("Calculate agrement and consistency")
let agreement = Double(totalMatches) / Double(totalVerdicts) * 100
print("consistencies: \(consistencies)")
var sumConsistency = 0.0
for value in consistencies {
    sumConsistency += value
}
print("sum: \(sumConsistency)")
let avgConsistency = sumConsistency / Double(consistencies.count)
print("avg: \(avgConsistency)")
print("")
print("Agreement with labels: \(totalMatches)/\(totalVerdicts) = \(String(format: "%.1f", agreement))%")
print("Average consistency: \(String(format: "%.1f", avgConsistency))%")
print("False passes: \(falsePasses)")



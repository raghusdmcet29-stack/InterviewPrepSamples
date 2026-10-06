//
//  main.swift
//  EvalStep6-WordsAndDigits
//
//  Created by Anussha on 06/10/26.
//

import Foundation

struct TestCase {
    let question: String
    let accepted: [String]   // accepted[0] is the plain/digit form
}

let testCases: [TestCase] = [
    TestCase(question: "What is the capital of France?", accepted: ["Paris"]),
    TestCase(question: "What is the capital of Japan?", accepted: ["Tokyo"]),
    TestCase(question: "What is the capital of Italy?", accepted: ["Rome"]),
    TestCase(question: "What is 7 times 8?", accepted: ["56", "fifty-six", "fifty six"]),
    TestCase(question: "What is 9 times 6?", accepted: ["54", "fifty-four", "fifty four"]),
    TestCase(question: "What is 12 plus 13?", accepted: ["25", "twenty-five", "twenty five"]),
    TestCase(question: "What is 100 minus 37?", accepted: ["63", "sixty-three", "sixty three"]),
    TestCase(question: "What is the capital of Canada?", accepted: ["Ottawa"]),
    TestCase(question: "How many days are in a week?", accepted: ["7", "seven"]),
    TestCase(question: "How many sides does a triangle have?", accepted: ["3", "three"])
]

func normalize(_ text: String) -> String {
    var result = text.precomposedStringWithCompatibilityMapping.lowercased()
    let dashes = ["\u{2010}", "\u{2011}", "\u{2012}", "\u{2013}", "\u{2014}", "\u{2212}"]
    for dash in dashes {
        result = result.replacingOccurrences(of: dash, with: "-")
    }
    result = result.replacingOccurrences(of: "-", with: " ")
    result = result.replacingOccurrences(of: "\\s+", with: " ", options: .regularExpression)
    return result.trimmingCharacters(in: .whitespacesAndNewlines)
}

func matchesWholeWord(response: String, form: String) -> Bool {
    let cleanResponse = normalize(response)
    let escaped = NSRegularExpression.escapedPattern(for: normalize(form))
    let pattern = "(?<![a-z0-9])" + escaped + "(?![a-z0-9])"
    return cleanResponse.range(of: pattern, options: .regularExpression) != nil
}

func wordsAndDigitsScore(response: String, accepted: [String]) -> Int {
    for form in accepted {
        if matchesWholeWord(response: response, form: form) {
            return 1
        }
    }
    return 0
}

func digitsOnlyScore(response: String, accepted: [String]) -> Int {
    return matchesWholeWord(response: response, form: accepted[0]) ? 1 : 0
}

func passRateReport(scores: [Int]) -> String {
    let total = scores.count
    guard total > 0 else { return "No test cases to score" }
    let passes = scores.reduce(0, +)
    let rate = Double(passes) / Double(total) * 100
    return "\(passes)/\(total) = " + String(format: "%.1f", rate) + "%"
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

func runEval(prompt: String, useWords: Bool) async -> [Int] {
    var scores: [Int] = []
    for testCase in testCases {
        let response = await callGroq(question: testCase.question, prompt: prompt)
        let score: Int
        if useWords {
            //print(response,testCase.accepted)
            score = wordsAndDigitsScore(response: response, accepted: testCase.accepted)
        } else {
            score = digitsOnlyScore(response: response, accepted: testCase.accepted)
        }
        scores.append(score)
        let verdict = score == 1 ? "PASS" : "FAIL"
        print("\(verdict): \(testCase.question) -> \(response)")
    }
    return scores
}
/*
// Offline checks first (no API call)
let acc = ["56", "fifty-six", "fifty six"]
print("1:", wordsAndDigitsScore(response: "The answer is 56.", accepted: acc))
print("2:", wordsAndDigitsScore(response: "Fifty-six.", accepted: acc))
print("3:", wordsAndDigitsScore(response: "156", accepted: acc))
print("4:", wordsAndDigitsScore(response: "Fifty six", accepted: acc))
print("5:", digitsOnlyScore(response: "Fifty-six.", accepted: acc))
*/
let promptA = "Answer briefly."
let promptB = "Answer in words only, never use digits."

let aScores = await runEval(prompt: promptA, useWords: true)
print("Prompt A, words+digits scorer: " + passRateReport(scores: aScores))

let bOldScores = await runEval(prompt: promptB, useWords: false)
print("Prompt B, digits-only scorer: " + passRateReport(scores: bOldScores))

let bNewScores = await runEval(prompt: promptB, useWords: true)
print("Prompt B, words+digits scorer: " + passRateReport(scores: bNewScores))

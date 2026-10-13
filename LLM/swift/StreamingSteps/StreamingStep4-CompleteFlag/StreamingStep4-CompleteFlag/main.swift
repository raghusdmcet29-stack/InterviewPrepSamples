//
//  main.swift
//  StreamingStep4-CompleteFlag
//
//  Created by Anussha on 13/10/26.
//

import Foundation

struct StreamResult {
    let text: String
    let complete: Bool
}

func pieceFrom(_ line: String) -> String? {
    guard line.hasPrefix("data: "), !line.contains("[DONE]") else {
        return nil
    }
    let jsonText = String(line.dropFirst("data: ".count))
    guard let data = jsonText.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
          let choices = obj["choices"] as? [[String: Any]],
          let delta = choices.first?["delta"] as? [String: Any],
          let content = delta["content"] as? String else {
        return nil
    }
    return content
}

func streamReply(_ prompt: String) async -> StreamResult {
    let apiKey = ProcessInfo.processInfo.environment["GROQ_API_KEY"] ?? ""
    let url = URL(string: "https://api.groq.com/openai/v1/chat/completions")!

    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    request.setValue("application/json", forHTTPHeaderField: "Content-Type")
    request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")

    let body: [String: Any] = [
        "model": "openai/gpt-oss-120b",
        "temperature": 0,
        "stream": true,
        "messages": [["role": "user", "content": prompt]]
    ]

    var full = ""
    var sawDone = false
    do {
        request.httpBody = try JSONSerialization.data(withJSONObject: body)
        let (bytes, _) = try await URLSession.shared.bytes(for: request)
        for try await line in bytes.lines {
            if line == "data: [DONE]" {
                sawDone = true
                break
            }
            if let piece = pieceFrom(line) {
                full += piece
                print("So far: \(full)")
            }
        }
    } catch {
        print("Stream error: \(error)")
    }
    return StreamResult(text: full, complete: sawDone)
}

Task {
    let key = ProcessInfo.processInfo.environment["GROQ_API_KEY"] ?? ""
    if key.isEmpty {
        print("GROQ_API_KEY is not set")
        exit(0)
    }
    let result = await streamReply("Say hello in one short sentence.")
    print("Text: \(result.text), complete: \(result.complete)")
    exit(0)
}
RunLoop.main.run()


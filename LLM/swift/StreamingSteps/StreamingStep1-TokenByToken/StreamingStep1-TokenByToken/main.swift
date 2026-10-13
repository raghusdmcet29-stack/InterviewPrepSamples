//
//  main.swift
//  StreamingStep1-TokenByToken
//
//  Created by Anussha on 13/10/26.
//


import Foundation

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

print(pieceFrom("data: {\"choices\":[{\"delta\":{\"content\":\"Hel\"}}]}") ?? "nil")
print(pieceFrom("data: [DONE]") ?? "nil")



/*
import Foundation

func showPieces(_ pieces: [String]) {
    var full = ""
    for piece in pieces {
        full += piece
        print("So far: \(full)")
    }
    print("Done")
}

showPieces(["Hel", "lo ", "there"])
*/

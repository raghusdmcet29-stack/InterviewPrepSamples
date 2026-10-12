//
//  main.swift
//  StructuredOutputStep2-FieldValidation
//
//  Created by Anussha on 12/10/26.
//


import Foundation

func rejectionReason(_ json: String) -> String {
    guard let data = json.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
        return "not valid JSON"
    }
    guard let name = obj["name"] as? String, let age = obj["age"] as? Int else {
        return "missing name or age"
    }
    if name.trimmingCharacters(in: .whitespaces).isEmpty {
        return "empty name"
    }
    if age < 0 || age > 120 {
        return "age out of range"
    }
    return "accepted"
}

func fakeModel(prompt: String) -> String {
    if prompt.contains("was rejected") {
        return "{\"name\": \"Ravi\", \"age\": 34}"
    }
    return "{\"name\": \"Ravi\", \"age\": -4}"
}

func getValidReply(question: String, maxTries: Int) -> String? {
    var prompt = question
    for i in 1...maxTries {
        let reply = fakeModel(prompt: prompt)
        let reason = rejectionReason(reply)
        print("Attempt \(i): \(reason)")
        if reason == "accepted" {
            return reply
        }
        prompt = question + " Your last reply was rejected because: \(reason). Fix it and reply with JSON only."
    }
    return nil
}

if let final = getValidReply(question: "Give name and age as JSON.", maxTries: 3) {
    print("Final: \(final)")
} else {
    print("No valid reply")
}

/*
import Foundation

func rejectionReason(_ json: String) -> String {
    guard let data = json.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
        return "not valid JSON"
    }
    guard let name = obj["name"] as? String, let age = obj["age"] as? Int else {
        return "missing name or age"
    }
    if name.trimmingCharacters(in: .whitespaces).isEmpty {
        return "empty name"
    }
    if age < 0 || age > 120 {
        return "age out of range"
    }
    return "accepted"
}

var attempt = 0
func fakeModel() -> String {
    attempt += 1
    if attempt == 1 {
        return "{\"name\": \"Ravi\", \"age\": -4}"
    }
    return "{\"name\": \"Ravi\", \"age\": 34}"
}

func getValidReply(maxTries: Int) -> String? {
    for i in 1...maxTries {
        let reply = fakeModel()
        let reason = rejectionReason(reply)
        print("Attempt \(i): \(reason)")
        if reason == "accepted" {
            return reply
        }
    }
    return nil
}

if let final = getValidReply(maxTries: 3) {
    print("Final: \(final)")
} else {
    print("No valid reply")
}
*/
/*
import Foundation

func isValidName(_ name: String) -> Bool {
    return !name.trimmingCharacters(in: .whitespaces).isEmpty
}

func isValidAge(_ age: Int) -> Bool {
    return age >= 0 && age <= 120
}

func isAccepted(_ json: String) -> Bool {
    guard let data = json.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
          let name = obj["name"] as? String,
          let age = obj["age"] as? Int else {
        return false
    }
    return isValidName(name) && isValidAge(age)
}

print(isAccepted("{\"name\": \"Ravi\", \"age\": 34}"))
print(isAccepted("{\"name\": \"\", \"age\": 34}"))
print(isAccepted("{\"name\": \"Ravi\", \"age\": -4}"))
print(isAccepted("not json"))
*/

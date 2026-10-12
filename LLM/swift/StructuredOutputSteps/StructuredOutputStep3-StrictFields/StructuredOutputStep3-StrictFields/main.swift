//
//  main.swift
//  StructuredOutputStep3-StrictFields
//
//  Created by Anussha on 12/10/26.
//

import Foundation

let allowedKeys: Set<String> = ["name", "age"]

func rejectionReason(_ json: String) -> String {
    guard let data = json.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
        return "not valid JSON"
    }
    if Set(obj.keys) != allowedKeys {
        return "unexpected or missing fields"
    }
    guard let name = obj["name"] as? String, let age = obj["age"] as? Int else {
        return "wrong types for name or age"
    }
    if name.trimmingCharacters(in: .whitespaces).isEmpty {
        return "empty name"
    }
    if age < 0 || age > 120 {
        return "age out of range"
    }
    return "accepted"
}

print(rejectionReason("{\"name\": \"Ravi\", \"age\": 34}"))
print(rejectionReason("{\"name\": \"Ravi\", \"age\": 34, \"city\": \"Bengaluru\"}"))
print(rejectionReason("{\"name\": \"Ravi\"}"))
print(rejectionReason("{\"name\": \"\", \"age\": 34}"))
print(rejectionReason("{\"name\": \"Ravi\", \"age\": -4}"))
print(rejectionReason("not json"))


/*
import Foundation

let allowedKeys: Set<String> = ["name", "age"]

func hasOnlyAllowedKeys(_ json: String) -> Bool {
    guard let data = json.data(using: .utf8),
          let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
        return false
    }
    return Set(obj.keys) == allowedKeys
}

print(hasOnlyAllowedKeys("{\"name\": \"Ravi\", \"age\": 34}"))
print(hasOnlyAllowedKeys("{\"name\": \"Ravi\", \"age\": 34, \"city\": \"Bengaluru\"}"))
print(hasOnlyAllowedKeys("{\"name\": \"Ravi\"}"))
*/

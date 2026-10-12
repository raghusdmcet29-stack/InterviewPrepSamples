//
//  main.swift
//  StructuredOutputStep5-Pipeline
//
//  Created by Anussha on 12/10/26.
//

import Foundation

struct Person: Codable {
    let name: String
    let age: Int
}

struct Rejection: Error {
    let reason: String
}

let allowedKeys: Set<String> = ["name", "age"]

func validate(_ json: String) -> Result<Person, Rejection> {
    guard let data = json.data(using: .utf8) else {
        return .failure(Rejection(reason: "bad encoding"))
    }
    guard let obj = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
        return .failure(Rejection(reason: "not valid JSON"))
    }
    if Set(obj.keys) != allowedKeys {
        return .failure(Rejection(reason: "unexpected or missing fields"))
    }
    guard let person = try? JSONDecoder().decode(Person.self, from: data) else {
        return .failure(Rejection(reason: "wrong types for name or age"))
    }
    if person.name.trimmingCharacters(in: .whitespaces).isEmpty {
        return .failure(Rejection(reason: "empty name"))
    }
    if person.age < 0 || person.age > 120 {
        return .failure(Rejection(reason: "age out of range"))
    }
    return .success(person)
}

func show(_ json: String) {
    switch validate(json) {
    case .success(let p):
        print("accepted: \(p.name), \(p.age)")
    case .failure(let rejection):
        print("rejected: \(rejection.reason)")
    }
}

show("{\"name\": \"Ravi\", \"age\": 34}")
show("{\"name\": \"Ravi\", \"age\": 34, \"city\": \"Bengaluru\"}")
show("{\"name\": \"Ravi\", \"age\": \"thirty\"}")
show("{\"name\": \"\", \"age\": 34}")
show("not json")

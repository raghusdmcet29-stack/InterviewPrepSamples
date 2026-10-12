//
//  main.swift
//  StructuredOutputStep4-TypedParsing
//
//  Created by Anussha on 12/10/26.
//

import Foundation

struct Person: Codable {
    let name: String
    let age: Int
}

func parse(_ json: String) {
    guard let data = json.data(using: .utf8) else {
        print("bad encoding")
        return
    }
    do {
        let person = try JSONDecoder().decode(Person.self, from: data)
        print("decoded: \(person.name), \(person.age)")
    } catch {
        print("decode failed")
    }
}

parse("{\"name\": \"Ravi\", \"age\": 34}")
parse("{\"name\": \"Ravi\"}")
parse("{\"name\": \"Ravi\", \"age\": \"thirty\"}")
parse("{\"name\": \"Ravi\", \"age\": 34, \"city\": \"Bengaluru\"}")


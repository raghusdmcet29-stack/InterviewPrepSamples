//
//  main.swift
//  FactoryMethodPattern
//
//  Created by Anussha on 11/09/26.
//

import Foundation

protocol Parser {
    func parse(_ content: String) -> String
}

struct PDFParser: Parser {
    func parse(_ content: String) -> String {
        return "Parsed as PDF: \(content)"
    }
}

struct CSVParser: Parser {
    func parse(_ content: String) -> String {
        return "Parsed as CSV: \(content)"
    }
}

struct JSONParser: Parser {
    func parse(_ content: String) -> String {
        return "Parsed as JSON: \(content)"
    }
}

enum ParserFactory {
    static func makeParser(for fileExtension: String) -> Parser? {
        switch fileExtension {
        case "pdf":
            return PDFParser()
        case "csv":
            return CSVParser()
        case "json":
            return JSONParser()
        default:
            return nil
        }
    }
}

let extensions = ["pdf", "csv", "json", "txt"]

for ext in extensions {
    if let parser = ParserFactory.makeParser(for: ext) {
        print(parser.parse("sample content"))
    } else {
        print("No parser available for .\(ext)")
    }
}


//
//  main.swift
//  AbstractFactoryPattern
//
//  Created by Anussha on 11/09/26.
//

import Foundation

protocol Parser {
    func parse(_ content: String) -> String
}

protocol Validator {
    func validate(_ content: String) -> Bool
}

struct PDFParser: Parser {
    func parse(_ content: String) -> String {
        return "Parsed as PDF: \(content)"
    }
}

struct PDFValidator: Validator {
    func validate(_ content: String) -> Bool {
        return content.hasPrefix("%PDF")
    }
}

struct CSVParser: Parser {
    func parse(_ content: String) -> String {
        return "Parsed as CSV: \(content)"
    }
}

struct CSVValidator: Validator {
    func validate(_ content: String) -> Bool {
        return content.contains(",")
    }
}

protocol FileToolkitFactory {
    func makeParser() -> Parser
    func makeValidator() -> Validator
}

struct PDFToolkitFactory: FileToolkitFactory {
    func makeParser() -> Parser { PDFParser() }
    func makeValidator() -> Validator { PDFValidator() }
}

struct CSVToolkitFactory: FileToolkitFactory {
    func makeParser() -> Parser { CSVParser() }
    func makeValidator() -> Validator { CSVValidator() }
}

func process(using factory: FileToolkitFactory, content: String) {
    let parser = factory.makeParser()
    let validator = factory.makeValidator()

    if validator.validate(content) {
        print(parser.parse(content))
    } else {
        print("Validation failed for: \(content)")
    }
}

process(using: PDFToolkitFactory(), content: "%PDF-1.4 sample data")
process(using: PDFToolkitFactory(), content: "not a pdf")
process(using: CSVToolkitFactory(), content: "a,b,c")
process(using: CSVToolkitFactory(), content: "no commas here")


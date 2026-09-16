//
//  main.swift
//  PrototypePattern
//
//  Created by Anussha on 16/09/26.
//

import Foundation

protocol Cloneable {
    func clone() -> Self
}

final class ReportTemplate : Cloneable {
    var header : String
    var footer : String
    var bodyStyle : String
    
    init(header: String, footer: String, bodyStyle: String) {
        self.header = header
        self.footer = footer
        self.bodyStyle = bodyStyle
    }
    
    func clone() -> Self {
        // Manual deep copy — without this, copying the reference
        // would just give you a second pointer to the SAME object.
        let copy = ReportTemplate(header: header, footer: footer, bodyStyle: bodyStyle)
        return copy as! Self
    }
}

// The "prototype" — one fully-configured instance we'll clone from
let defaultReport = ReportTemplate(
    header: "Company Confidential",
    footer: "Page {n}",
    bodyStyle: "Arial 12pt"
)

//Clone it instead of constructing from scratch
let q3Report = defaultReport.clone()
q3Report.header = "Q3 Financial Report"

print("Prototype header: \(defaultReport.header)")
print("Clone header: \(q3Report.header)")


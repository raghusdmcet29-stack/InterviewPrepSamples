//
//  main.swift
//  TemplateMethodPattern
//
//  Created by Anussha on 22/09/26.
//

import Foundation

class ReportGenerator {
    // The template method — fixed sequence, never overridden
    
    final func generateReport() {
            loadData()
            processData()
            formatOutput()
            saveOutput()
        }
    
    // Now locked — cannot be overridden by any subclass
       final func loadData() {
            print("Loading raw data...")
        }
    
        final func processData() {
            print("Processing data (cleaning, aggregating)...")
        }
    
    // Steps that vary — subclasses MUST override these
        func formatOutput() {
            fatalError("Subclass must override formatOutput()")
        }

        func saveOutput() {
            fatalError("Subclass must override saveOutput()")
        }
}
class PDFReportGenerator: ReportGenerator {
    override func formatOutput() {
        print("Formatting output as PDF (styled text, headers, page breaks)...")
    }

    override func saveOutput() {
        print("Saving output as report.pdf")
    }
   
}

class CSVReportGenerator: ReportGenerator {
    override func formatOutput() {
        print("Formatting output as CSV (comma-separated rows)...")
    }

    override func saveOutput() {
        print("Saving output as report.csv")
    }
}

let pdfReport = PDFReportGenerator()
pdfReport.generateReport()
let csvReport = CSVReportGenerator()
csvReport.generateReport()

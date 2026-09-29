//
//  main.cpp
//  TemplateMethodPattern
//
//  Created by Anussha on 22/09/26.
//

#include <iostream>

class ReportGenerator {
public:
    // Non-virtual — this is the fixed skeleton, cannot be overridden
    void generateReport() {
        loadData();
        processData();
        formatOutput();
        saveOutput();
    }

    // Shared steps — also non-virtual, same reasoning as Swift's `final`
    void loadData() {
        std::cout << "Loading raw data..." << std::endl;
    }

    void processData() {
        std::cout << "Processing data (cleaning, aggregating)..." << std::endl;
    }

    // Steps that vary — must be virtual so subclasses can override
    virtual void formatOutput() = 0;
    virtual void saveOutput() = 0;

    virtual ~ReportGenerator() = default;
};

class PDFReportGenerator : public ReportGenerator {
public:
    
    void formatOutput() override {
        std::cout << "Formatting output as PDF (styled text, headers, page breaks)..." << std::endl;
    }

    void saveOutput() override {
        std::cout << "Saving output as report.pdf" << std::endl;
    }
};

class CSVReportGenerator : public ReportGenerator {
public:
    void formatOutput() override {
        std::cout << "Formatting output as CSV (comma-separated rows)..." << std::endl;
    }

    void saveOutput() override {
        std::cout << "Saving output as report.csv" << std::endl;
    }
};

int main(){
    ReportGenerator* pdf = new PDFReportGenerator();
    pdf->generateReport();
    delete pdf;
    
    ReportGenerator* csv = new CSVReportGenerator();
    csv->generateReport();
    delete csv;
    return 0;
}

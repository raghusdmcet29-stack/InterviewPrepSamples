//
//  main.c
//  TemplateMethodPattern
//
//  Created by Anussha on 22/09/26.
//

#include <stdio.h>

// The varying steps — each report type supplies its own pair
typedef struct {
    void (*formatOutput)(void);
    void (*saveOutput)(void);
} ReportOps;

// The fixed skeleton — takes the varying ops as a parameter
void generateReport(ReportOps *ops) {
    // Shared steps — hardcoded here, not customizable at all
    printf("Loading raw data...\n");
    printf("Processing data (cleaning, aggregating)...\n");

    // Varying steps — dispatched through the function pointers
    ops->formatOutput();
    ops->saveOutput();
}

// PDF variant
void pdfFormatOutput(void) {
    printf("Formatting output as PDF (styled text, headers, page breaks)...\n");
}

void pdfSaveOutput(void) {
    printf("Saving output as report.pdf\n");
}

ReportOps pdfOps = { pdfFormatOutput, pdfSaveOutput };

// CSV variant
void csvFormatOutput(void) {
    printf("Formatting output as CSV (comma-separated rows)...\n");
}

void csvSaveOutput(void) {
    printf("Saving output as report.csv\n");
}

ReportOps csvOps = { csvFormatOutput, csvSaveOutput };

int main(void) {
    generateReport(&pdfOps);
    generateReport(&csvOps);
    return 0;
}

//
//  main.c
//  PrototypePattern
//
//  Created by Anussha on 16/09/26.
//

#include <string.h>
#include <stdio.h>
#define FIELD_SIZE 64

typedef struct {
    char header[FIELD_SIZE];
    char footer[FIELD_SIZE];
    char bodyStyle[FIELD_SIZE];
} ReportTemplate;

ReportTemplate clone_report(const ReportTemplate* prototype) {
    ReportTemplate copy;
    strcpy(copy.header, prototype->header);
    strcpy(copy.footer, prototype->footer);
    strcpy(copy.bodyStyle, prototype->bodyStyle);
    return copy;
}

int main(void){
    ReportTemplate defaultReport;
    strcpy(defaultReport.header, "Company Confidential");
    strcpy(defaultReport.footer, "Page {n}");
    strcpy(defaultReport.bodyStyle, "Arial 12pt");

    ReportTemplate q3Report = clone_report(&defaultReport);
    strcpy(q3Report.header, "Q3 Financial Report");

    printf("Prototype header: %s\n", defaultReport.header);
    printf("Clone header: %s\n", q3Report.header);

       return 0;
}

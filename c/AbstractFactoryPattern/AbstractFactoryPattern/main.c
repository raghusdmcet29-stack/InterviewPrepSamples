//
//  main.c
//  AbstractFactoryPattern
//
//  Created by Anussha on 11/09/26.
//
#include <stdio.h>
#include <string.h>

typedef char* (*ParseFn)(const char* content);
typedef int (*ValidateFn)(const char* content);

char* pdf_parse(const char* content) {
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "Parsed as PDF: %s", content);
    return buffer;
}

int pdf_validate(const char* content) {
    return strncmp(content, "%PDF", 4) == 0;
}

char* csv_parse(const char* content) {
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "Parsed as CSV: %s", content);
    return buffer;
}

int csv_validate(const char* content) {
    return strchr(content, ',') != NULL;
}

typedef struct {
    ParseFn parse;
    ValidateFn validate;
} FileToolkitFactory;

FileToolkitFactory make_pdf_factory(void) {
    FileToolkitFactory f;
    f.parse = pdf_parse;
    f.validate = pdf_validate;
    return f;
}

FileToolkitFactory make_csv_factory(void) {
    FileToolkitFactory f;
    f.parse = csv_parse;
    f.validate = csv_validate;
    return f;
}

void process(FileToolkitFactory factory, const char* content) {
    if (factory.validate(content)) {
        printf("%s\n", factory.parse(content));
    } else {
        printf("Validation failed for: %s\n", content);
    }
}

int main(void) {
    FileToolkitFactory pdfFactory = make_pdf_factory();
    FileToolkitFactory csvFactory = make_csv_factory();

    process(pdfFactory, "%PDF-1.4 sample data");
    process(pdfFactory, "not a pdf");
    process(csvFactory, "a,b,c");
    process(csvFactory, "no commas here");

    return 0;
}

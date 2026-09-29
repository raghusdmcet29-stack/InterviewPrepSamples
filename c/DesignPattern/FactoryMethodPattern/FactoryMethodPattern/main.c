//
//  main.c
//  FactoryMethodPattern
//
//  Created by Anussha on 11/09/26.
//

#include <stdio.h>
#include <string.h>

typedef struct Parser {
    char* (*parse)(const char* content);
} Parser;

char* pdf_parse(const char* content) {
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "Parsed as PDF: %s", content);
    return buffer;
}

char* csv_parse(const char* content) {
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "Parsed as CSV: %s", content);
    return buffer;
}

char* json_parse(const char* content) {
    static char buffer[128];
    snprintf(buffer, sizeof(buffer), "Parsed as JSON: %s", content);
    return buffer;
}

Parser make_parser(const char* file_extension) {
    Parser p;
    if (strcmp(file_extension, "pdf") == 0) {
        p.parse = pdf_parse;
    } else if (strcmp(file_extension, "csv") == 0) {
        p.parse = csv_parse;
    } else if (strcmp(file_extension, "json") == 0) {
        p.parse = json_parse;
    } else {
        p.parse = NULL;
    }
    return p;
}

int main(void) {
    const char* extensions[] = {"pdf", "csv", "json", "txt"};

    for (int i = 0; i < 4; i++) {
        Parser p = make_parser(extensions[i]);
        if (p.parse != NULL) {
            printf("%s\n", p.parse("sample content"));
        } else {
            printf("No parser available for .%s\n", extensions[i]);
        }
    }

    return 0;
}

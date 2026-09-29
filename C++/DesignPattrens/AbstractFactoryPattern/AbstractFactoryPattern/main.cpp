//
//  main.cpp
//  AbstractFactoryPattern
//
//  Created by Anussha on 11/09/26.
//

#include <iostream>
#include <memory>
#include <string>

class Parser {
public:
    virtual std::string parse(const std::string& content) = 0;
    virtual ~Parser() = default;
};

class Validator {
public:
    virtual bool validate(const std::string& content) = 0;
    virtual ~Validator() = default;
};

class PDFParser : public Parser {
public:
    std::string parse(const std::string& content) override {
        return "Parsed as PDF: " + content;
    }
};

class PDFValidator : public Validator {
public:
    bool validate(const std::string& content) override {
        return content.rfind("%PDF", 0) == 0; // starts with "%PDF"
    }
};

class CSVParser : public Parser {
public:
    std::string parse(const std::string& content) override {
        return "Parsed as CSV: " + content;
    }
};

class CSVValidator : public Validator {
public:
    bool validate(const std::string& content) override {
        return content.find(',') != std::string::npos;
    }
};

class FileToolkitFactory {
public:
    virtual std::unique_ptr<Parser> makeParser() = 0;
    virtual std::unique_ptr<Validator> makeValidator() = 0;
    virtual ~FileToolkitFactory() = default;
};

class PDFToolkitFactory : public FileToolkitFactory {
public:
    std::unique_ptr<Parser> makeParser() override {
        return std::make_unique<PDFParser>();
    }
    std::unique_ptr<Validator> makeValidator() override {
        return std::make_unique<PDFValidator>();
    }
};

class CSVToolkitFactory : public FileToolkitFactory {
public:
    std::unique_ptr<Parser> makeParser() override {
        return std::make_unique<CSVParser>();
    }
    std::unique_ptr<Validator> makeValidator() override {
        return std::make_unique<CSVValidator>();
    }
};

void process(FileToolkitFactory& factory, const std::string& content) {
    auto parser = factory.makeParser();
    auto validator = factory.makeValidator();

    if (validator->validate(content)) {
        std::cout << parser->parse(content) << std::endl;
    } else {
        std::cout << "Validation failed for: " << content << std::endl;
    }
}

int main() {
    PDFToolkitFactory pdfFactory;
    CSVToolkitFactory csvFactory;

    process(pdfFactory, "%PDF-1.4 sample data");
    process(pdfFactory, "not a pdf");
    process(csvFactory, "a,b,c");
    process(csvFactory, "no commas here");

    return 0;
}

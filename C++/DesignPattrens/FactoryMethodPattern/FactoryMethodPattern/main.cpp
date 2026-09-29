//
//  main.cpp
//  FactoryMethodPattern
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

class PDFParser : public Parser {
public:
    std::string parse(const std::string& content) override {
        return "Parsed as PDF: " + content;
    }
};


class CSVParser : public Parser {
public:
    std::string parse(const std::string& content) override {
        return "Parsed as CSV: " + content;
    }
};

class JSONParser : public Parser {
public:
    std::string parse(const std::string& content) override {
        return "Parsed as JSON: " + content;
    }
};

class ParserFactory {
public:
    static std::unique_ptr<Parser> makeParser(const std::string& fileExtension) {
        if (fileExtension == "pdf") {
            return std::make_unique<PDFParser>();
        } else if (fileExtension == "csv") {
            return std::make_unique<CSVParser>();
        } else if (fileExtension == "json") {
            return std::make_unique<JSONParser>();
        } else {
            return nullptr;
        }
    }
};

int main() {
    std::vector<std::string> extensions = {"pdf", "csv", "json", "txt"};

    for (const auto& ext : extensions) {
        auto parser = ParserFactory::makeParser(ext);
        if (parser) {
            std::cout << parser->parse("sample content") << std::endl;
        } else {
            std::cout << "No parser available for ." << ext << std::endl;
        }
    }

    return 0;
}

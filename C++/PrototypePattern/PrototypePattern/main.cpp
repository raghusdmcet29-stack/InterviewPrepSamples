//
//  main.cpp
//  PrototypePattern
//
//  Created by Anussha on 16/09/26.
//

#include <iostream>
#include <string>
#include <memory>

class Cloneable {
public:
    virtual std::unique_ptr<Cloneable> clone() const = 0;
    virtual ~Cloneable() = default;
};

class ReportTemplate : public Cloneable{
public:
    std::string header;
    std::string footer;
    std::string bodyStyle;
    
    ReportTemplate(std::string header, std::string footer, std::string bodyStyle)
        : header(std::move(header)), footer(std::move(footer)), bodyStyle(std::move(bodyStyle)) {}
    std::unique_ptr<Cloneable> clone() const override {
            // Uses the compiler-generated copy constructor under the hood —
            // std::string members already deep-copy themselves correctly.
            return std::make_unique<ReportTemplate>(*this);
        }
};

int main() {
    ReportTemplate defaultReport("Company Confidential", "Page {n}", "Arial 12pt");

    std::unique_ptr<Cloneable> cloned = defaultReport.clone();
    // Need to get back to ReportTemplate to touch its specific fields
    ReportTemplate* q3Report = static_cast<ReportTemplate*>(cloned.get());
    q3Report->header = "Q3 Financial Report";

    std::cout << "Prototype header: " << defaultReport.header << "\n";
    std::cout << "Clone header: " << q3Report->header << "\n";

    return 0;
}

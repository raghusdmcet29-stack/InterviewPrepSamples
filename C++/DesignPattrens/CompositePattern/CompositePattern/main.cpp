//
//  main.cpp
//  CompositePattern
//
//  Created by Anussha on 09/09/26.
//

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class FileSystemComponent {
public:
    virtual std::string getName() const = 0;
    virtual int getSize() const = 0;
    virtual ~FileSystemComponent() = default;
};

class File : public FileSystemComponent {
private:
    std::string name;
    int size;

public:
    File(const std::string& name, int size) : name(name), size(size) {}

    std::string getName() const override { return name; }
    int getSize() const override { return size; }
};

class Folder : public FileSystemComponent {
private:
    std::string name;
    std::vector<std::unique_ptr<FileSystemComponent>> children;

public:
    Folder(const std::string& name) : name(name) {}

    void add(std::unique_ptr<FileSystemComponent> component) {
        children.push_back(std::move(component));
    }

    std::string getName() const override { return name; }

    int getSize() const override {
        int total = 0;
        for (const auto& child : children) {
            total += child->getSize();
        }
        return total;
    }
};

int main() {
    auto documents = std::make_unique<Folder>("Documents");
    documents->add(std::make_unique<File>("resume.pdf", 120));
    documents->add(std::make_unique<File>("photo.jpg", 300));

    auto archive = std::make_unique<Folder>("Archive");
    archive->add(std::make_unique<File>("old_report.docx", 80));

    documents->add(std::move(archive));

    std::cout << documents->getName() << " total size: " << documents->getSize() << " KB\n";
}

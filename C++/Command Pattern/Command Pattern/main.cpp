//
//  main.cpp
//  Command Pattern
//
//  Created by Anussha on 04/09/26.
//

#include <string>
#include <iostream>

// The receiver - does the actual work
class TextBuffer {
public:
    void insert(const std::string& s) {
        text += s;
    }
    
    void deleteChars(size_t count) {
        text.erase(text.size() - count, count);
    }
    
    const std::string& getText() const {
        return text;
    }
    
private:
    std::string text;
};

// The Command interface - abstract base class, not a template
class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;   // virtual destructor - required since we'll delete through base pointers
};

class InsertCommand : public Command {
public:
    InsertCommand(TextBuffer& buffer, const std::string& text)
        : buffer(buffer), textToInsert(text) {}
    
    void execute() override {
        buffer.insert(textToInsert);
    }
    
    void undo() override {
        buffer.deleteChars(textToInsert.size());
    }
    
private:
    TextBuffer& buffer;
    std::string textToInsert;
};

class DeleteCommand : public Command {
public:
    DeleteCommand(TextBuffer& buffer, size_t count)
        : buffer(buffer), count(count) {}
    
    void execute() override {
        deletedText = buffer.getText().substr(buffer.getText().size() - count, count);
        buffer.deleteChars(count);
    }
    
    void undo() override {
        buffer.insert(deletedText);
    }
    
private:
    TextBuffer& buffer;
    size_t count;
    std::string deletedText;   // captured at execute() time, same wrinkle as the Swift version
};

#include <vector>
#include <memory>

class Invoker {
public:
    void run(std::unique_ptr<Command> command) {
        command->execute();
        history.push_back(std::move(command));
    }
    
    void undoLast() {
        if (history.empty()) {
            std::cout << "Nothing to undo" << std::endl;
            return;
        }
        history.back()->undo();
        history.pop_back();
    }
    
private:
    std::vector<std::unique_ptr<Command>> history;
};

int main() {
    TextBuffer buffer;
    Invoker invoker;
    
    invoker.run(std::make_unique<InsertCommand>(buffer, "Hello"));
    std::cout << buffer.getText() << std::endl;   // "Hello"
    
    invoker.run(std::make_unique<InsertCommand>(buffer, " World"));
    std::cout << buffer.getText() << std::endl;   // "Hello World"
    
    invoker.run(std::make_unique<DeleteCommand>(buffer, 6));
    std::cout << buffer.getText() << std::endl;   // "Hello"   (deleted " World")
    
    invoker.undoLast();
    std::cout << buffer.getText() << std::endl;   // "Hello World"   (undid the delete)
    
    invoker.undoLast();
    std::cout << buffer.getText() << std::endl;   // "Hello"   (undid second insert)
    
    invoker.undoLast();
    std::cout << buffer.getText() << std::endl;   // ""   (undid first insert)
    
    invoker.undoLast();   // "Nothing to undo" - stack empty
    
    return 0;
}

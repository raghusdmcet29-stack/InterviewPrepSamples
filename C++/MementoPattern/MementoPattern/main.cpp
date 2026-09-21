//
//  main.cpp
//  MementoPattern
//
//  Created by Anussha on 21/09/26.
//

#include <iostream>
#include <string>


class TextEditorMemento {
public:
    explicit TextEditorMemento(std::string content) : content(std::move(content)) {}

private:
    std::string content;
    std::string getContent() const { return content; }

    friend class TextEditor;  // only TextEditor can call getContent()
};

class TextEditor {
public:
    void type(const std::string& text) {
        content += text;
    }

    std::string getContent() const {
        return content;
    }

    TextEditorMemento save() const {
        return TextEditorMemento(content);
    }

    void restore(const TextEditorMemento& memento) {
        content = memento.getContent();  // allowed only because of friend
    }

private:
    std::string content;
};

class Caretaker {
public:
    void save(const TextEditorMemento& memento) {
        history.push_back(memento);
    }

    bool undo(TextEditorMemento& outMemento) {
        if (history.empty()) return false;
        outMemento = history.back();
        history.pop_back();
        return true;
    }

private:
    std::vector<TextEditorMemento> history;
};



int main() {
    TextEditor editor1;

    editor1.type("Hello");
    TextEditorMemento saved = editor1.save();
    std::cout << "After typing 'Hello': " << editor1.getContent() << "\n";

    editor1.type(" World");
    std::cout << "After typing ' World': " << editor1.getContent() << "\n";

    editor1.restore(saved);
    std::cout << "After restore: " << editor1.getContent() << "\n";
    
    TextEditor editor;
        Caretaker caretaker;

        editor.type("A");
        caretaker.save(editor.save());   // checkpoint 1: "A"

        editor.type("B");
        caretaker.save(editor.save());   // checkpoint 2: "AB"

        editor.type("C");
        std::cout << "Before undo: " << editor.getContent() << "\n";   // ABC

        TextEditorMemento memento("");  // placeholder, will be overwritten
        if (caretaker.undo(memento)) {
            editor.restore(memento);
        }
        std::cout << "After 1 undo: " << editor.getContent() << "\n";  // AB

        if (caretaker.undo(memento)) {
            editor.restore(memento);
        }
        std::cout << "After 2nd undo: " << editor.getContent() << "\n"; // A

    return 0;
}

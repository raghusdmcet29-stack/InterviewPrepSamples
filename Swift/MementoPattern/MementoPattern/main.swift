//
//  main.swift
//  MementoPattern
//
//  Created by Anussha on 21/09/26.
//

import Foundation

struct TextEditorMemento{
    let content : String
}

class TextEditor {
    private(set) var content : String = ""
    
    func type(_ text : String){
        content += text
    }
    
    func save() -> TextEditorMemento {
        return TextEditorMemento(content: content)
    }
    
    func restore(from momento: TextEditorMemento){
        content = momento.content
    }
}

class Caretaker {
    private var history: [TextEditorMemento] = []

    func save(_ memento: TextEditorMemento) {
        history.append(memento)
    }

    func undo() -> TextEditorMemento? {
        guard !history.isEmpty else { return nil }
        return history.removeLast()
    }
}

let editor = TextEditor()

editor.type("Hello")
let saved = editor.save()
print("After typing 'Hello':", editor.content)

editor.type(" World")
print("After typing ' World':", editor.content)

editor.restore(from: saved)
print("After restore:", editor.content)

let editor2 = TextEditor()
let caretaker = Caretaker()

editor2.type("A")
caretaker.save(editor2.save())   // checkpoint 1: "A"

editor2.type("B")
caretaker.save(editor2.save())   // checkpoint 2: "AB"

editor2.type("C")
print("Before undo:", editor2.content)   // ABC

if let last = caretaker.undo() {
    editor2.restore(from: last)
}
print("After 1 undo:", editor2.content)  // back to AB

if let prev = caretaker.undo() {
    editor2.restore(from: prev)
}
print("After 2nd undo:", editor2.content) // back to A

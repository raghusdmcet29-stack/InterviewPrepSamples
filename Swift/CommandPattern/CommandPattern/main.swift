//
//  main.swift
//  CommandPattern
//
//  Created by Anussha on 04/09/26.
//

import Foundation

class TextBuffer {
    private(set) var text : String = ""
    
    func insert(_ string : String){
        text += string
    }
    
    func delete(_ count : Int){
        text = String(text.dropLast(count))
    }
}

// The Command protocol - every command knows how to execute AND undo itself
protocol Command {
    func execute()
    func undo()
}

class InsertCommand : Command{
    private let buffer : TextBuffer
    private let textToInsert : String
    
    init(buffer: TextBuffer, text: String) {
        self.buffer = buffer
        self.textToInsert = text
    }
    
    func execute() {
            buffer.insert(textToInsert)
        }
        
        func undo() {
            buffer.delete(textToInsert.count)
        }
}

class DeleteCommand: Command {
    private let buffer: TextBuffer
    private let count: Int
    private var deletedText: String = ""   // captured at execute() time, needed for undo
    
    init(buffer: TextBuffer, count: Int) {
        self.buffer = buffer
        self.count = count
    }
    
    func execute() {
        deletedText = String(buffer.text.suffix(count))
        buffer.delete(count)
    }
    
    func undo() {
        buffer.insert(deletedText)
    }
}

class Invoker {
    private var history: [Command] = []
    
    func run(_ command: Command) {
        command.execute()
        history.append(command)
    }
    
    func undoLast() {
        guard let lastCommand = history.popLast() else {
            print("Nothing to undo")
            return
        }
        lastCommand.undo()
    }
}

let buffer = TextBuffer()
let invoker = Invoker()

invoker.run(InsertCommand(buffer: buffer, text: "Hello"))
print(buffer.text)   // "Hello"

invoker.run(InsertCommand(buffer: buffer, text: " World"))
print(buffer.text)   // "Hello World"

invoker.run(DeleteCommand(buffer: buffer, count: 6))
print(buffer.text)   // "Hello"   (deleted " World")

invoker.undoLast()
print(buffer.text)   // "Hello World"   (undid the delete -> reinserted " World")

invoker.undoLast()
print(buffer.text)   // "Hello"   (undid the second insert)

invoker.undoLast()
print(buffer.text)   // ""   (undid the first insert)

invoker.undoLast()   // "Nothing to undo" - stack empty


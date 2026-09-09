//
//  main.swift
//  CompositePattern
//
//  Created by Anussha on 09/09/26.
//

import Foundation

protocol FileSystemComponent {
    var name: String { get }
    func getSize() -> Int
}

class File: FileSystemComponent {
    let name: String
    let size: Int

    init(name: String, size: Int) {
        self.name = name
        self.size = size
    }

    func getSize() -> Int {
        return size
    }
}

class Folder: FileSystemComponent {
    let name: String
    private var children: [FileSystemComponent] = []

    init(name: String) {
        self.name = name
    }

    func add(_ component: FileSystemComponent) {
        children.append(component)
    }

    func getSize() -> Int {
        return children.reduce(0) { $0 + $1.getSize() }
    }
}

let file1 = File(name: "resume.pdf", size: 120)
let file2 = File(name: "photo.jpg", size: 300)

print("\(file1.name): \(file1.getSize()) KB")
print("\(file2.name): \(file2.getSize()) KB")

let documents = Folder(name: "Documents")
documents.add(file1)
documents.add(file2)

let nestedFolder = Folder(name: "Archive")
nestedFolder.add(File(name: "old_report.docx", size: 80))

documents.add(nestedFolder)

print("\(documents.name) total size: \(documents.getSize()) KB")


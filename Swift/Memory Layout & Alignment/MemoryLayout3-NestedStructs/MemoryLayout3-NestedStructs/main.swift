//
//  main.swift
//  MemoryLayout3-NestedStructs
//
//  Created by Anussha on 09/10/26.
//

struct Inner {
    var a: Int8
    var b: Int32
}

struct Outer {
    var x: Int8
    var inner: Inner
}

print("Inner: size \(MemoryLayout<Inner>.size), alignment \(MemoryLayout<Inner>.alignment), stride \(MemoryLayout<Inner>.stride)")
print("Outer: size \(MemoryLayout<Outer>.size), alignment \(MemoryLayout<Outer>.alignment), stride \(MemoryLayout<Outer>.stride)")


//
//  main.swift
//  MemoryLayout2-StructPadding
//
//  Created by Anussha on 08/10/26.
//

struct Example {
    var a: Int8
    var b: Int32
}

print("Example size:", MemoryLayout<Example>.size, "alignment:", MemoryLayout<Example>.alignment)

struct Mixed {
    var a: Int8
    var b: Int32
    var c: Int8
}

struct Reordered {
    var b: Int32
    var a: Int8
    var c: Int8
}

print("Mixed     size:", MemoryLayout<Mixed>.size, "stride:", MemoryLayout<Mixed>.stride, "alignment:", MemoryLayout<Mixed>.alignment)
print("Reordered size:", MemoryLayout<Reordered>.size, "stride:", MemoryLayout<Reordered>.stride, "alignment:", MemoryLayout<Reordered>.alignment)

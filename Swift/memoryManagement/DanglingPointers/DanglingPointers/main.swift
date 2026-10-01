//
//  main.swift
//  DanglingPointers
//
//  Created by Anussha on 01/10/26.
//

import Foundation
// fix for dangling pointer
var p: UnsafeMutablePointer<Int>? = UnsafeMutablePointer<Int>.allocate(capacity: 1)
p?.initialize(to: 42)

print("Before free: \(p!.pointee)")

p?.deallocate()
let q = p  // before set nil
p = nil
print(" before free: \(q!.pointee)")  // again garbase value becasue we set nill p not q.
print("Freed.")

if let r = p {
    print("After free: \(r.pointee)")
} else {
    print("Pointer is nil, read skipped")
}

// below is the code which creats dangling pointer
/*
let p = UnsafeMutablePointer<Int>.allocate(capacity: 1)
p.initialize(to: 42)

print("Before free: \(p.pointee)")

p.deallocate()

print("Freed.")
print("After free: \(p.pointee)")

*/

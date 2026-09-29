//
//  main.swift
//  Copy-on-Write internals
//
//  Created by Anussha on 29/09/26.
//

import Foundation

func address(of array: [Int]) -> String {
    return array.withUnsafeBufferPointer { buffer in
        return "\(buffer.baseAddress!)"
    }
}

var a = [1, 2, 3]
var b = a

print("a:", address(of: a))
print("b:", address(of: b))
b.append(4)

print("a after append:", address(of: a))
print("b after append:", address(of: b))

var c = [1, 2, 3]
print("c before append:", address(of: c))

c.append(4)
c.append(5)

print("c after append:", address(of: c))
c.append(6)
print("c after more than 3 elements append:", address(of: c))
c.append(7)
print("c after more than 4th elements append:", address(of: c))
var d = [1, 2, 3]
d.reserveCapacity(10)
print("d capacity:", d.capacity)
print("d before append:", address(of: d))

d.append(4)
print("d after append:", address(of: d))

var e = [1, 2, 3]
for n in 4...13 {
    e.append(n)
    print("count:", e.count, "capacity:", e.capacity)
}

var f = [1, 2, 3]
f.reserveCapacity(10)
var g = f
print("f:", address(of: f))
print("g:", address(of: g))

g.append(4)
print("f after:", address(of: f))
print("g after:", address(of: g))
print("f capacity:", f.capacity)
print("g capacity:", g.capacity)

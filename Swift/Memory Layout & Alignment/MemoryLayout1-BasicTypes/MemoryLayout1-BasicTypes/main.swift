//
//  main.swift
//  MemoryLayout1-BasicTypes
//
//  Created by Anussha on 08/10/26.
//

import Foundation

print("Int8   size:", MemoryLayout<Int8>.size,   "alignment:", MemoryLayout<Int8>.alignment)
print("Int32  size:", MemoryLayout<Int32>.size,  "alignment:", MemoryLayout<Int32>.alignment)
print("Int64  size:", MemoryLayout<Int64>.size,  "alignment:", MemoryLayout<Int64>.alignment)
print("Bool   size:", MemoryLayout<Bool>.size,   "alignment:", MemoryLayout<Bool>.alignment)
print("Double size:", MemoryLayout<Double>.size, "alignment:", MemoryLayout<Double>.alignment)


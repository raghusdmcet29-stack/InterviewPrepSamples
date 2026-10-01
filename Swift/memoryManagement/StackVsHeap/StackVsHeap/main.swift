//
//  main.swift
//  StackVsHeap
//
//  Created by Anussha on 01/10/26.
//

class Box {
    var value = 1
    deinit { print("Box freed") }
}

func makeBox() -> Box {
    let box = Box()
    print("inside makeBox:  \(Unmanaged.passUnretained(box).toOpaque())")
    print("makeBox ending")
    return box
}

func caller() {
    let b = makeBox()
    print("outside makeBox: \(Unmanaged.passUnretained(b).toOpaque())")
    print("caller ending")
}

caller()
print("after caller")
/*
class Box {
    var value = 1
    deinit { print("Box freed") }
}

func run() {
    var x = 10
    let box = Box()

    withUnsafePointer(to: &x) { print("x address:   \($0)") }
    print("box address: \(Unmanaged.passUnretained(box).toOpaque())")
    print("run() ending")
}

run()
print("--- second call ---")
run()
run()
*/

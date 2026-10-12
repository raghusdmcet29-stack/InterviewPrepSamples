//
//  main.swift
//  ThreadLocalStorage
//
//  Created by Anussha on 12/10/26.
//

import Foundation

func work(_ name: String) {
    Thread.current.threadDictionary["name"] = name
    sleep(1)
    let mine = Thread.current.threadDictionary["name"] as! String
    print("\(name) reads \(mine)")
}

let group = DispatchGroup()
for name in ["T1", "T2", "T3"] {
    group.enter()
    DispatchQueue.global().async {
        work(name)
        group.leave()
    }
}
group.wait()
print("Done")


/*
import Foundation

var sharedName = ""

func work(_ name: String) {
    sharedName = name
    sleep(1)
    print("\(name) reads \(sharedName)")
}

let group = DispatchGroup()
for name in ["T1", "T2", "T3"] {
    group.enter()
    DispatchQueue.global().async {
        work(name)
        group.leave()
    }
}
group.wait()
print("Done")
*/

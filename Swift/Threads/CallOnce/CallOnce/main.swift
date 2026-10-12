//
//  main.swift
//  CallOnce
//
//  Created by Anussha on 12/10/26.
//
// multiple loding problem solved using call once

import Foundation

var config: String? = nil
let configLock = NSLock()

func loadConfig(_ who: String) -> String {
    print("\(who) is loading config")
    sleep(1)
    return "settings"
}

func getConfig(_ who: String) -> String {
    if let c = config {
        return c
    }
    configLock.lock()
    defer { configLock.unlock() }
    if config == nil {
        config = loadConfig(who)
    }
    return config!
}

let group = DispatchGroup()
for name in ["T1", "T2", "T3"] {
    group.enter()
    DispatchQueue.global().async {
        _ = getConfig(name)
        group.leave()
    }
}
group.wait()
print("Done")


/*import Foundation

var config: String? = nil
let configLock = NSLock()

func loadConfig(_ who: String) -> String {
    print("\(who) is loading config")
    sleep(1)
    return "settings"
}

func getConfig(_ who: String) -> String {
    configLock.lock()
    defer { configLock.unlock() }
    if config == nil {
        config = loadConfig(who)
    }
    return config!
}

let group = DispatchGroup()
for name in ["T1", "T2", "T3"] {
    group.enter()
    DispatchQueue.global().async {
        _ = getConfig(name)
        group.leave()
    }
}
group.wait()
print("Done")
*/

// multiple loding problem
/*
import Foundation

var config: String? = nil

func loadConfig(_ who: String) -> String {
    print("\(who) is loading config")
    sleep(1)
    return "settings"
}

func getConfig(_ who: String) -> String {
    if config == nil {
        config = loadConfig(who)
    }
    return config!
}

let group = DispatchGroup()
for name in ["T1", "T2", "T3"] {
    group.enter()
    DispatchQueue.global().async {
        _ = getConfig(name)
        group.leave()
    }
}
group.wait()
print("Done")

*/

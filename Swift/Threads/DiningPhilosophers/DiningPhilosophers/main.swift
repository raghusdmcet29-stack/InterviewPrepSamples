//
//  main.swift
//  DiningPhilosophers
//
//  Created by Anussha on 07/10/26.
//

import Foundation

let forks = [NSLock(), NSLock(), NSLock()]
let group = DispatchGroup()
// avoiding dead lock

func philosopher(_ id: Int) {
    let left = id
    let right = (id + 1) % 3
    let first = min(left, right)
    let second = max(left, right)
    forks[first].lock()
    print("P\(id) picked up F\(first)")
    sleep(1)
    print("P\(id) wants F\(second)")
    forks[second].lock()
    print("P\(id) eating")
    forks[second].unlock()
    forks[first].unlock()
}

//creates dead lock
/*
func philosopher(_ id: Int) {
    let left = id
    let right = (id + 1) % 3
    forks[left].lock()
    print("P\(id) picked up F\(left)")
    sleep(1)
    print("P\(id) wants F\(right)")
    forks[right].lock()
    print("P\(id) eating")
    forks[right].unlock()
    forks[left].unlock()
}*/

for id in 0..<3 {
    group.enter()
    DispatchQueue.global().async {
        philosopher(id)
        group.leave()
    }
}

group.wait()
print("Done")


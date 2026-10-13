//
//  main.swift
//  ThreadConfinement
//
//  Created by Anussha on 13/10/26.
//

import Foundation

let line = DispatchQueue(label: "counter.queue")
var counter = 0   // only touched inside `line`

for _ in 0..<1000 {
    line.async {
        counter += 1
    }
}

line.sync {
    print("Counter: \(counter)")
}


/*
import Foundation

let line = DispatchQueue(label: "counter.queue")

line.async {
    print("Task 1 start")
    sleep(2)
    print("Task 1 end")
}
line.async {
    print("Task 2 start")
}
line.async {
    print("Task 3 start")
}

sleep(5)   // keeps the program alive long enough for the tasks to finish

*/

//
//  main.swift
//  Semaphore
//
//  Created by Anussha on 08/10/26.
//

import Foundation

//let carPark = DispatchSemaphore(value: 2)
let carPark = DispatchSemaphore(value: 1)
let group = DispatchGroup()

for name in ["A", "B", "C"] {
    group.enter()
    DispatchQueue.global().async {
        carPark.wait()
        print("Car \(name) parked")
        sleep(2)
        print("Car \(name) left")
        carPark.signal()
        group.leave()
    }
}

group.wait()
print("Done")

/*let carPark = DispatchSemaphore(value: 2)

carPark.wait()
print("Car A parked")

carPark.wait()
print("Car B parked")

carPark.signal()
print("Car A left")

carPark.wait()
print("Car C parked")
*/

//
//  main.swift
//  TelemetryPipeline
//
//  Created by Anussha on 14/10/26.
//

import Foundation

let queue = BoundedQueue<Reading>(capacity: 4)
let stats = StatsStore()
let workersDone = DispatchGroup()
let dashboardDone = DispatchGroup()

for id in 1...2 {
    let worker = Worker(id: id, queue: queue, stats: stats)
    DispatchQueue.global().async(group: workersDone) {
        worker.run()
    }
}

DispatchQueue.global().async(group: dashboardDone) {
    for tick in 1...5 {
        usleep(20_000)
        for device in ["sensor-1", "sensor-2"] {
            if let s = stats.snapshot(for: device) {
                print("dashboard tick \(tick): \(device) count=\(s.count)")
            }
        }
    }
}

for value in [21.0, 23.0, 22.0] {
    queue.enqueue(Reading(deviceID: "sensor-1", value: value))
    queue.enqueue(Reading(deviceID: "sensor-2", value: value + 10))
}

queue.close()
workersDone.wait()
dashboardDone.wait()

for device in ["sensor-1", "sensor-2"] {
    if let s = stats.snapshot(for: device) {
        print("\(device) count=\(s.count) min=\(s.min) max=\(s.max) avg=\(s.average)")
    }
}

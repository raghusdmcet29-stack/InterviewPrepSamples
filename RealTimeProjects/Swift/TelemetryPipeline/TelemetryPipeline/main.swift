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

for id in 1...2 {
    let worker = Worker(id: id, queue: queue, stats: stats)
    DispatchQueue.global().async(group: workersDone) {
        worker.run()
    }
}

for value in [21.0, 23.0, 22.0] {
    queue.enqueue(Reading(deviceID: "sensor-1", value: value))
    queue.enqueue(Reading(deviceID: "sensor-2", value: value + 10))
}

queue.close()
workersDone.wait()

for device in ["sensor-1", "sensor-2"] {
    if let s = stats.snapshot(for: device) {
        print("\(device) count=\(s.count) min=\(s.min) max=\(s.max) avg=\(s.average)")
    }
}

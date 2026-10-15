//
//  Worker.swift
//  TelemetryPipeline
//
//  Created by Anussha on 14/10/26.
//

import Foundation

final class Worker{
    private let id : Int
    private let queue : BoundedQueue<Reading>
    private let stats: StatsStore
    
    init(id: Int, queue: BoundedQueue<Reading>, stats: StatsStore) {
        self.id = id
        self.queue = queue
        self.stats = stats
    }
    
    func run() {
        while let reading = queue.dequeue() {
            print("worker \(id) processed \(reading.deviceID): \(reading.value)")
            stats.record(reading)
        }
        print("worker \(id) stopped")
    }
}

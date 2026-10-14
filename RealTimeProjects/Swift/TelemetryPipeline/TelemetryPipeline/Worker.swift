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
    
    init(id : Int,queue : BoundedQueue<Reading>){
        self.id = id
        self.queue = queue
    }
    
    func run() {
        while let reading = queue.dequeue() {
            print("worker \(id) processed \(reading.deviceID): \(reading.value)")
        }
        print("worker \(id) stopped")
    }
}

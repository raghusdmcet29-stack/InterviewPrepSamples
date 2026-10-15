//
//  StatsStore.swift
//  TelemetryPipeline
//
//  Created by Anussha on 14/10/26.
//

import Foundation

final class StatsStore {
    private var stats: [String: DeviceStats] = [:]
    private let lockQueue = DispatchQueue(label: "stats", attributes: .concurrent)
   
    
    func record(_ reading: Reading) {
        lockQueue.sync(flags: .barrier) {
            var s = stats[reading.deviceID] ?? DeviceStats()
            s.add(reading.value)
            stats[reading.deviceID] = s
        }
    }

    func snapshot(for deviceID: String) -> DeviceStats? {
        lockQueue.sync {
            stats[deviceID]
        }
    }
}

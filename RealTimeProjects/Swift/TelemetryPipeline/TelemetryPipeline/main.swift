//
//  main.swift
//  TelemetryPipeline
//
//  Created by Anussha on 14/10/26.
//

import Foundation

let store = StatsStore()
store.record(Reading(deviceID: "sensor-1", value: 21.0))
store.record(Reading(deviceID: "sensor-1", value: 23.0))
store.record(Reading(deviceID: "sensor-1", value: 22.0))

if let s = store.snapshot(for: "sensor-1") {
    print("sensor-1 count=\(s.count) min=\(s.min) max=\(s.max) avg=\(s.average)")
}

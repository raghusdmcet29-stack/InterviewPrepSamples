//
//  Stats.swift
//  TelemetryPipeline
//
//  Created by Anussha on 14/10/26.
//

import Foundation

struct DeviceStats {
    private(set) var count = 0
    private(set) var min = Double.greatestFiniteMagnitude
    private(set) var max = -Double.greatestFiniteMagnitude
    private(set) var sum = 0.0
    
    var average : Double{
        count == 0 ? 0 : sum/Double(count)
    }
    
    mutating func add(_ value : Double){
        count += 1
        sum += value
        if value < min { min = value }
        if value > max { max = value }
    }
}

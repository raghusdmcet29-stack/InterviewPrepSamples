//
//  main.swift
//  AdapterPattern
//
//  Created by Anussha on 08/09/26.
//

import Foundation

// The interface client code expects
protocol TemperatureSensor {
    func readCelsius() -> Double
}


// Legacy class we can't modify — returns Fahrenheit as a formatted string
class LegacyFahrenheitSensor {
    func getTemperature() -> String {
        // simulate a hardware reading
        return "98.6F"
    }
}

class FahrenheitToCelsiusAdapter: TemperatureSensor {
    private let legacySensor: LegacyFahrenheitSensor
    
    init(legacySensor: LegacyFahrenheitSensor) {
            self.legacySensor = legacySensor
        }
    
    func readCelsius() -> Double {
        let raw = legacySensor.getTemperature() // e.g. "98.6F"
        print("raw:\(raw)")
        let numericPart = raw.dropLast()
        print("numericPart:\(numericPart)")
        guard let fahrenheit = Double(numericPart) else {
            return 0.0 // fallback for malformed data
        }// "98.6"
        print("fahrenheit:\(fahrenheit)")
        return (fahrenheit - 32) * 5 / 9
    }
}

func printReport(sensor: TemperatureSensor) {
    let celsius = sensor.readCelsius()
    print("Current temperature: \(celsius)°C")
}

// Wire it up
let legacy = LegacyFahrenheitSensor()
let adapter = FahrenheitToCelsiusAdapter(legacySensor: legacy)

printReport(sensor: adapter)


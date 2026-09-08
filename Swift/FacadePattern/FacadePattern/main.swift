//
//  main.swift
//  FacadePattern
//
//  Created by Anussha on 08/09/26.
//

import Foundation

class Amplifier {
    func on() { print("Amplifier on") }
    func setVolume(_ level: Int) { print("Amplifier volume set to \(level)") }
    func off() { print("Amplifier off") }
}

class DVDPlayer {
    func on() { print("DVD Player on") }
    func play(_ movie: String) { print("DVD Player playing \"\(movie)\"") }
    func stop() { print("DVD Player stopped") }
    func off() { print("DVD Player off") }
}

class Projector {
    func on() { print("Projector on") }
    func setInput(_ source: String) { print("Projector input set to \(source)") }
    func off() { print("Projector off") }
}

class HomeTheaterFacade {
    private let amp: Amplifier
    private let dvd: DVDPlayer
    private let projector: Projector
    
    init(amp: Amplifier, dvd: DVDPlayer, projector: Projector) {
            self.amp = amp
            self.dvd = dvd
            self.projector = projector
        }
    
    func watchMovie(_ movie: String) {
            print("--- Setting up movie night ---")
            amp.on()
            amp.setVolume(5)
            projector.on()
            projector.setInput("DVD")
            dvd.on()
            dvd.play(movie)
        }
    
    func endMovie() {
            print("--- Shutting down movie night ---")
            dvd.stop()
            dvd.off()
            projector.off()
            amp.off()
        }
}

let amp = Amplifier()
let dvd = DVDPlayer()
let projector = Projector()

let homeTheater = HomeTheaterFacade(amp: amp, dvd: dvd, projector: projector)

homeTheater.watchMovie("Inception")
print()
homeTheater.endMovie()


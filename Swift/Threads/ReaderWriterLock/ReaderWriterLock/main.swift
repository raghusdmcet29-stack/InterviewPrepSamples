//
//  main.swift
//  ReaderWriterLock
//
//  Created by Anussha on 06/10/26.
//
// parallel running
import Foundation

final class Settings {
    private var theme = "light"
    private let queue = DispatchQueue(label: "settings.rw", attributes: .concurrent)

    func readTheme(reader: Int) -> String {
        return queue.sync {
            print("Reader \(reader) started")
            sleep(1)
            let value = theme
            print("Reader \(reader) finished")
            return value
        }
    }
    
    func writeTheme(_ newTheme: String) {
        queue.sync(flags: .barrier) {
            print("Writer started")
            sleep(1)
            theme = newTheme
            print("Writer finished")
        }
    }
}

let settings = Settings()
let group = DispatchGroup()
let start = Date()

for reader in 1...2 {
    group.enter()
    DispatchQueue.global().async {
        let value = settings.readTheme(reader: reader)
        print("Reader \(reader) saw \(value)")
        group.leave()
    }
}

usleep(200_000)
group.enter()
DispatchQueue.global().async {
    settings.writeTheme("dark")
    group.leave()
}

usleep(200_000)
group.enter()
DispatchQueue.global().async {
    let value = settings.readTheme(reader: 3)
    print("Reader 3 saw \(value)")
    group.leave()
}

group.wait()
let elapsed = Date().timeIntervalSince(start)
print("Done in " + String(format: "%.1f", elapsed) + " seconds")


/*
let settings = Settings()
let group = DispatchGroup()
let start = Date()

for reader in 1...3 {
    group.enter()
    DispatchQueue.global().async {
        _ = settings.readTheme(reader: reader)
        group.leave()
    }
}
group.wait()

let elapsed = Date().timeIntervalSince(start)
print("Done in " + String(format: "%.1f", elapsed) + " seconds")
*/
/*
 // seqencial runnint mutiple threads
import Foundation

final class Settings {
    private var theme = "light"
    private let lock = NSLock()

    func readTheme(reader: Int) -> String {
        lock.lock()
        print("Reader \(reader) started")
        sleep(1)
        let value = theme
        print("Reader \(reader) finished")
        lock.unlock()
        return value
    }
}

let settings = Settings()
let group = DispatchGroup()
let start = Date()

for reader in 1...3 {
    group.enter()
    DispatchQueue.global().async {
        _ = settings.readTheme(reader: reader)
        group.leave()
    }
}
group.wait()

let elapsed = Date().timeIntervalSince(start)
print("Done in " + String(format: "%.1f", elapsed) + " seconds")


*/

/*
import Foundation

final class Settings {
    private var theme = "light"

    func readTheme(reader: Int) -> String {
        print("Reader \(reader) started")
        sleep(1)
        let value = theme
        print("Reader \(reader) finished")
        return value
    }
}

let settings = Settings()
let value = settings.readTheme(reader: 1)
print("Reader 1 saw \(value)")
print("Done")

*/

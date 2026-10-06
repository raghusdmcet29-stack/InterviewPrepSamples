//
//  main.swift
//  ThreadPool
//
//  Created by Anussha on 06/10/26.
//

import Foundation

struct Job {
    let id: Int

    func run() {
        print("Job \(id) started")
        sleep(1)
        print("Job \(id) finished")
    }
}

final class JobQueue {
    private var jobs: [Job] = []
    private var closed = false
    private let cond = NSCondition()

    func add(_ job: Job) {
        cond.lock()
        jobs.append(job)
        cond.signal()
        cond.unlock()
    }

    func close() {
        cond.lock()
        closed = true
        cond.broadcast()
        cond.unlock()
    }

    func take() -> Job? {
        cond.lock()
        while jobs.isEmpty && !closed {
            cond.wait()
        }
        if jobs.isEmpty {
            cond.unlock()
            return nil
        }
        let job = jobs.removeFirst()
        cond.unlock()
        return job
    }
}


let queue = JobQueue()
for id in 1...10 {
    queue.add(Job(id: id))
}
queue.close()

let group = DispatchGroup()
for w in 1...3 {
    group.enter()
    DispatchQueue.global().async {
        while let job = queue.take() {
            job.run()
        }
        print("Worker \(w) quitting")
        group.leave()
    }
}
group.wait()
print("Done")

/*let queue = JobQueue()
queue.add(Job(id: 1))
queue.add(Job(id: 2))
queue.add(Job(id: 3))
queue.close()

let group = DispatchGroup()
group.enter()
DispatchQueue.global().async {
    while let job = queue.take() {
        job.run()
    }
    print("Worker quitting")
    group.leave()
}
group.wait()
print("Done")

*/


/*
import Foundation

struct Job {
    let id: Int

    func run() {
        print("Job \(id) started")
        sleep(1)
        print("Job \(id) finished")
    }
}

final class JobQueue {
    private var jobs: [Job] = []
    private let cond = NSCondition()

    func add(_ job: Job) {
        cond.lock()
        jobs.append(job)
        cond.signal()
        cond.unlock()
    }

    func take() -> Job {
        cond.lock()
        while jobs.isEmpty {
            cond.wait()
        }
        let job = jobs.removeFirst()
        cond.unlock()
        return job
    }
}


let queue = JobQueue()
for id in 1...6 {
    queue.add(Job(id: id))
}

let group = DispatchGroup()
for _ in 1...3 {
    group.enter()
    DispatchQueue.global().async {
        for _ in 1...2 {
            let job = queue.take()
            job.run()
        }
        group.leave()
    }
}
group.wait()
print("Done")

*/

/*
let queue = JobQueue()
queue.add(Job(id: 1))
queue.add(Job(id: 2))
queue.add(Job(id: 3))

let group = DispatchGroup()
group.enter()
DispatchQueue.global().async {
    for _ in 1...3 {
        let job = queue.take()
        job.run()
    }
    group.leave()
}
group.wait()
print("Done")

*/


/*
import Foundation

struct Job{
    let id : Int
    
    func run(){
        print("Job \(id) started")
        sleep(1)
        print("Job \(id) finished")
    }
}

final class JobQueue {
    private var jobs : [Job] = []
    private let cond = NSCondition()
    
    func add(_ job : Job){
        cond.lock()
        jobs.append(job)
        cond.signal()
        cond.unlock()
    }
    
    func take() -> Job {
        cond.lock()
        while jobs.isEmpty{
            cond.wait()
        }
        let job = jobs.removeFirst()
        cond.unlock()
        return job
    }
}

let queue = JobQueue()
queue.add(Job(id: 1))
queue.add(Job(id: 2))
queue.add(Job(id: 3))

for _ in 1...3 {
    let job = queue.take()
    job.run()
}
print("Done")*/
/*
import Foundation

struct Job {
    let id: Int

    func run() {
        print("Job \(id) started")
        sleep(1)
        print("Job \(id) finished")
    }
}

let job = Job(id: 1)
job.run()
print("Done")
*/

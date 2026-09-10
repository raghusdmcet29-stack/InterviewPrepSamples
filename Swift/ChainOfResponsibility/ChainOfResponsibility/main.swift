//
//  main.swift
//  ChainOfResponsibility
//
//  Created by Anussha on 10/09/26.
//

import Foundation

enum Priority {
    case low, medium, high
}

struct Ticket {
    let description: String
    let priority: Priority
}

protocol SupportHandler: AnyObject {
    var next: SupportHandler? { get set }
    func handle(ticket: Ticket)
}

class Level1Support : SupportHandler{
    var next : SupportHandler?
    
    func handle(ticket: Ticket) {
        if ticket.priority == .low {
            print("Level1Support resolved: \(ticket.description)")
        }else {
            print("Level1Support cannot resolve, escalating...")
            next?.handle(ticket: ticket)
        }
    }
}

class Level2Support: SupportHandler {
    var next: SupportHandler?

    func handle(ticket: Ticket) {
        if ticket.priority == .medium {
            print("Level2Support resolved: \(ticket.description)")
        } else {
            print("Level2Support cannot resolve, escalating...")
            next?.handle(ticket: ticket)
        }
    }
}

class Manager: SupportHandler {
    var next: SupportHandler?

    func handle(ticket: Ticket) {
        if ticket.priority == .high {
            print("Manager resolved: \(ticket.description)")
        } else {
            print("Manager cannot resolve, and no one left to escalate to.")
        }
    }
}

let level1 = Level1Support()
let level2 = Level2Support()
let manager = Manager()

level1.next = level2
level2.next = manager

let tickets = [
    Ticket(description: "Forgot password", priority: .low),
    Ticket(description: "App crashing intermittently", priority: .medium),
    Ticket(description: "Data breach reported", priority: .high)
]

for ticket in tickets {
    level1.handle(ticket: ticket)
}


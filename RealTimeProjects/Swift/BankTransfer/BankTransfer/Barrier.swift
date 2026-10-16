//
//  Barrier.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

import Foundation

final class Barrier {
    private let parties: Int
    private var waiting = 0
    private let condition = NSCondition()

    init(parties: Int) {
        self.parties = parties
    }

    func wait() {
        condition.lock()
        waiting += 1
        if waiting == parties {
            condition.broadcast()
        } else {
            while waiting < parties {
                condition.wait()
            }
        }
        condition.unlock()
    }
}

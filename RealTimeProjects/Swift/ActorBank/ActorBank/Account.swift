//
//  Account.swift
//  ActorBank
//
//  Created by Anussha on 19/10/26.
//

import Foundation

final class Account {
    let id: String
    private(set) var balance: Int
    private(set) var isFrozen: Bool = false

    init(id: String, balance: Int) {
        self.id = id
        self.balance = balance
    }

    func deposit(_ amount: Int) -> Bool {
        guard !isFrozen else { return false }
        balance += amount
        return true
    }

    func withdraw(_ amount: Int) -> Bool {
        guard amount <= balance else { return false }
        balance -= amount
        return true
    }

    func freeze() {
        isFrozen = true
    }
}

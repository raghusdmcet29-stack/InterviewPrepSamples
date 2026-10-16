//
//  Account.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//



import Foundation

final class Account {
    let id: Int
    var balance: Int
    let lock = NSLock()

    init(id: Int, balance: Int) {
        self.id = id
        self.balance = balance
    }
}

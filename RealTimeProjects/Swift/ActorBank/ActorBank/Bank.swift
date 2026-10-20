//
//  Bank.swift
//  ActorBank
//
//  Created by Anussha on 19/10/26.
//

import Foundation

actor Bank{
    private var accounts : [String : Account] = [:]
    
    func openAccount(id: String, balance : Int) -> Account{
        let account = Account(id: id, balance: balance)
        accounts[id] = account
        return account
    }
    
    func account(withId id : String) -> Account? {
        accounts[id]
    }
    
    func transfer(from fromId : String, to toId : String, amount : Int) async -> Bool{
        guard let from = accounts[fromId], let to = accounts[toId] else {
            return false
        }
        guard await from.withdraw(amount) else {
            return false
        }
        guard await to.deposit(amount) else {
            _ = await from.deposit(amount)
            return false
        }
        return true
    }
    
    func canTransfer(from fromId: String, to toId: String, amount: Int) async -> Bool {
        guard let from = accounts[fromId], let to = accounts[toId] else {
            return false
        }
        let fromBalance = await from.balance
        let toFrozen = await to.isFrozen
        return fromBalance >= amount && !toFrozen
    }
}

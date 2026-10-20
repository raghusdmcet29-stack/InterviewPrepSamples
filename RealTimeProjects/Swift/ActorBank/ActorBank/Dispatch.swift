//
//  Dispatch.swift
//  ActorBank
//
//  Created by Anussha on 20/10/26.
//

import Foundation

func run(_ command : Command,on bank : Bank) async -> String{
    switch command {
    case .deposit(let account,let amount):
        guard let acct = await bank.account(withId: account) else {
            return "unknown account \(account)"
        }
        let ok = await acct.deposit(amount)
        return ok ? "ok" : "refused"
    case .transfer(from: let from, to: let to, amount: let amount):
        let ok = await bank.transfer(from: from, to: to, amount: amount)
                return ok ? "ok" : "refused"
    case .balance(let account):
        guard let acct = await bank.account(withId: account) else {
            return "unknown account \(account)"
        }
        let amount = await acct.balance
        return String(amount)
    }
}

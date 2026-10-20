//
//  Dispatch.swift
//  ActorBank
//
//  Created by Anussha on 20/10/26.
//

func run(_ command: Command, on bank: Bank) async -> String {
    switch command {
    case .deposit(let account, let amount):
        guard await bank.balance(of: account) != nil else {
            return "unknown account \(account)"
        }
        let ok = await bank.deposit(account: account, amount: amount)
        return ok ? "ok" : "refused"

    case .transfer(let from, let to, let amount):
        let ok = await bank.transfer(from: from, to: to, amount: amount)
        return ok ? "ok" : "refused"

    case .balance(let account):
        guard let amount = await bank.balance(of: account) else {
            return "unknown account \(account)"
        }
        return String(amount)
    }
}

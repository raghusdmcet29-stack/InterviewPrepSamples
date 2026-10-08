//
//  main.swift
//  Actor-reentrancy
//
//  Created by Anussha on 08/10/26.

//an actor runs one function at a time, with one exception. When a function inside the actor hits an await, it pauses, and another caller is allowed in while it waits. This is called reentrancy.

import Foundation

actor Account {
    var balance = 100

    func saveToServer() async {
        try? await Task.sleep(nanoseconds: 1_000_000_000)
    }

    func withdraw(_ amount: Int, caller: String) async {
        if balance >= amount {
            print("\(caller) check passed, balance \(balance)")
            balance -= amount
            await saveToServer() // if you add this before witdrwal result will be diffrent.
            print("\(caller) withdrew \(amount), balance \(balance)")
        } else {
            print("\(caller) refused, balance \(balance)")
        }
    }
}

let account = Account()

await withTaskGroup(of: Void.self) { group in
    group.addTask { await account.withdraw(80, caller: "Caller 1") }
    group.addTask { await account.withdraw(80, caller: "Caller 2") }
}

print("Final balance \(await account.balance)")

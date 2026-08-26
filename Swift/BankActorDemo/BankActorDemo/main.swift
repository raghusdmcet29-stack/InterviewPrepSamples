//
//  main.swift
//  BankActorDemo
//
//  Created by Anussha on 25/08/26.
//

import Foundation

//class UnsafeBankAccount{
actor BankAccount{
    private var balance : Int
    
    init(balance: Int){
        self.balance = balance
    }
    
    func withdraw(_ amount:Int){
        let current = balance
        // simulate a tiny delay to widen the race window
        usleep(1000)
        balance = current - amount
    }
    func deposit(_ amount: Int) {
        balance += amount
    }
    
    var currentBalance: Int{
        balance
    }
}
/* class changed to actor so this code not needed
let account = UnsafeBankAccount(balance: 1000)

DispatchQueue.concurrentPerform(iterations: 100) { _ in
    account.withdraw(1)
}


print("Final balance: \(account.currentBalance)")*/

func performWithdrawal(on account: BankAccount) async {
    await account.withdraw(1)
}

func performDeposit(on account: BankAccount) async{
    await account.deposit(11)
}

let account = BankAccount(balance: 1000)

Task {
    await withTaskGroup(of: Void.self) { group in
        for _ in 0..<100 {
            group.addTask {
                await performWithdrawal(on: account)
            }
            group.addTask {
                await performDeposit(on: account)
            }
        }
    }
    
    let final = await account.currentBalance
    print("Final balance: \(final)")
    exit(0)
}

RunLoop.main.run()

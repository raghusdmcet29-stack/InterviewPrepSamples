//
//  Command.swift
//  ActorBank
//
//  Created by Anussha on 20/10/26.
//

import Foundation

enum Command{
    case deposit(account:String, amount : Int)
    case transfer(from: String, to: String, amount: Int)
    case balance(account: String)
}

func parseDeposit(_ line:String) -> Command? {
    let parts = line.split(separator: " ")
    guard parts.count == 3 else {
        print("expected 3 parts, got \(parts.count)")
        return nil
    }
    
    guard let amount = Int(parts[2]) else {
        print("amount is not a number: \(parts[2])")
        return nil
    }
    
    return .deposit(account: String(parts[1]), amount: amount)
}

func parseTransfer(_ line: String) -> Command? {
    let parts = line.split(separator: " ")
    
    guard parts.count == 4 else {
        print("expected 4 parts for transfer, got \(parts.count)")
        return nil
    }
    
    guard let amount = Int(parts[3]) else {
        print("amount is not a number: \(parts[3])")
        return nil
    }
    return .transfer(from: String(parts[1]), to: String(parts[2]), amount: amount)
}

func parseBalance(_ line: String) -> Command? {
    let parts = line.split(separator: " ")

    guard parts.count == 2 else {
        print("expected 2 parts for balance, got \(parts.count)")
        return nil
    }

    return .balance(account: String(parts[1]))
}



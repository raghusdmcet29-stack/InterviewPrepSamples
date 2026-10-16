//
//  AuditID.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

import Foundation

enum AuditID{
    static func set(_ id : String){
        Thread.current.threadDictionary["auditID"] = id
    }
    
    static func current() -> String{
        Thread.current.threadDictionary["auditID"] as? String ?? "none"
    }
}

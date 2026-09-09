//
//  main.swift
//  ProxyPattern
//
//  Created by Anussha on 09/09/26.
//

import Foundation

protocol DatabaseConnection {
    func query(_ sql: String) -> String
}

class RealDatabaseConnection : DatabaseConnection {
    init(){
        print("Connecting to database... (expensive setup)")
               // simulate expensive work
        Thread.sleep(forTimeInterval: 0.5)
        print("Connected.")
    }
    
    func query(_ sql: String) -> String {
            return "Result of '\(sql)'"
        }
}

class LazyDatabaseConnectionProxy: DatabaseConnection {
    private var realConnection: RealDatabaseConnection?

    func query(_ sql: String) -> String {
        if realConnection == nil {
            print("Proxy: no real connection yet, creating one now...")
            realConnection = RealDatabaseConnection()
        }
        return realConnection!.query(sql)
    }
}

print("Before creating connection")
let db = RealDatabaseConnection()
print("After creating connection")
print(db.query("SELECT * FROM users"))
print("Creating proxy...")
let proxyDb: DatabaseConnection = LazyDatabaseConnectionProxy()
print("Proxy created (no real connection yet)")

print(proxyDb.query("SELECT * FROM users"))
print(proxyDb.query("SELECT * FROM orders"))


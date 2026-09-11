//
//  main.swift
//  SingletonPattern
//
//  Created by Anussha on 11/09/26.
//

import Foundation

final class AppConfig {
    static let shared = AppConfig()

   /* private(set)*/ var environment: String
    /*private(set) */var maxRetries: Int

    private init() {
        environment = "production"
        maxRetries = 3
    }
}

print(AppConfig.shared.environment)
print(AppConfig.shared.maxRetries)

let a = AppConfig.shared
let b = AppConfig.shared
print(a === b) // should print true — same instance

a.maxRetries = 10
print(b.maxRetries) // if singleton is real, this should print 10, not 3

let c = AppConfig()


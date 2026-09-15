//
//  main.swift
//  BuilderPattern
//
//  Created by Anussha on 15/09/26.
//

import Foundation

struct HTTPRequest {
    let method: String
    let url: String
    let headers: [String: String]
    let queryParams: [String: String]
    let body: String?
}


class HTTPRequestBuilder {
    private var method: String = "GET"
    private var url: String = ""
    private var headers: [String: String] = [:]
    private var queryParams: [String: String] = [:]
    private var body: String?

    func setMethod(_ method: String) -> HTTPRequestBuilder {
        self.method = method
        return self
    }

    func setURL(_ url: String) -> HTTPRequestBuilder {
        self.url = url
        return self
    }

    func addHeader(_ key: String, _ value: String) -> HTTPRequestBuilder {
        headers[key] = value
        return self
    }

    func addQueryParam(_ key: String, _ value: String) -> HTTPRequestBuilder {
        queryParams[key] = value
        return self
    }

    func setBody(_ body: String) -> HTTPRequestBuilder {
        self.body = body
        return self
    }

    func build() -> HTTPRequest? {
        guard !url.isEmpty else {
            print("Build failed: URL is required")
            return nil
        }
        return HTTPRequest(
            method: method,
            url: url,
            headers: headers,
            queryParams: queryParams,
            body: body
        )
    }
}

let request = HTTPRequestBuilder()
    .setMethod("POST")
    .setURL("https://api.example.com/users")
    .addHeader("Content-Type", "application/json")
    .addHeader("Authorization", "Bearer token123")
    .addQueryParam("verbose", "true")
    .setBody("{\"name\":\"Anuraag\"}")
    .build()

if let request = request {
    print("Method: \(request.method)")
    print("URL: \(request.url)")
    print("Headers: \(request.headers)")
    print("Query Params: \(request.queryParams)")
    print("Body: \(request.body ?? "none")")
} else {
    print("Request build failed")
}

// Invalid case: no URL set
let badRequest = HTTPRequestBuilder()
    .setMethod("GET")
    .build()

print(badRequest == nil ? "Bad request correctly returned nil" : "Unexpected success")


//
//  main.cpp
//  BuilderPattern
//
//  Created by Anussha on 15/09/26.
//
#include <string>
#include <map>
#include <optional>
#include <iostream>

struct HTTPRequest {
    std::string method;
    std::string url;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> queryParams;
    std::optional<std::string> body;
};

class HTTPRequestBuilder {
private:
    std::string method_ = "GET";
    std::string url_;
    std::map<std::string, std::string> headers_;
    std::map<std::string, std::string> queryParams_;
    std::optional<std::string> body_;

public:
    HTTPRequestBuilder& setMethod(const std::string& method) {
        method_ = method;
        return *this;
    }

    HTTPRequestBuilder& setURL(const std::string& url) {
        url_ = url;
        return *this;
    }

    HTTPRequestBuilder& addHeader(const std::string& key, const std::string& value) {
        headers_[key] = value;
        return *this;
    }

    HTTPRequestBuilder& addQueryParam(const std::string& key, const std::string& value) {
        queryParams_[key] = value;
        return *this;
    }

    HTTPRequestBuilder& setBody(const std::string& body) {
        body_ = body;
        return *this;
    }

    std::optional<HTTPRequest> build() {
        if (url_.empty()) {
            std::cout << "Build failed: URL is required\n";
            return std::nullopt;
        }
        return HTTPRequest{method_, url_, headers_, queryParams_, body_};
    }
};

int main() {
    auto request = HTTPRequestBuilder()
        .setMethod("POST")
        .setURL("https://api.example.com/users")
        .addHeader("Content-Type", "application/json")
        .addHeader("Authorization", "Bearer token123")
        .addQueryParam("verbose", "true")
        .setBody("{\"name\":\"Anuraag\"}")
        .build();

    if (request.has_value()) {
        std::cout << "Method: " << request->method << "\n";
        std::cout << "URL: " << request->url << "\n";
        std::cout << "Headers:\n";
        for (const auto& [key, value] : request->headers) {
            std::cout << "  " << key << ": " << value << "\n";
        }
        std::cout << "Query Params:\n";
        for (const auto& [key, value] : request->queryParams) {
            std::cout << "  " << key << ": " << value << "\n";
        }
        std::cout << "Body: " << (request->body ? *request->body : "none") << "\n";
    } else {
        std::cout << "Request build failed\n";
    }

    // Invalid case: no URL set
    auto badRequest = HTTPRequestBuilder()
        .setMethod("GET")
        .build();

    std::cout << (badRequest.has_value() ? "Unexpected success" : "Bad request correctly returned nil") << "\n";

    return 0;
}

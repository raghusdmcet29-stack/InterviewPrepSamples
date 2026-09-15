//
//  main.c
//  BuilderPattern
//
//  Created by Anussha on 15/09/26.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_HEADERS 10
#define MAX_PARAMS 10

typedef struct {
    char key[64];
    char value[128];
} KeyValue;

typedef struct {
    char method[16];
    char url[256];
    KeyValue headers[MAX_HEADERS];
    int headerCount;
    KeyValue queryParams[MAX_PARAMS];
    int paramCount;
    char body[256];
    bool hasBody;
} HTTPRequest;


typedef struct {
    HTTPRequest request;
} HTTPRequestBuilder;

void builder_init(HTTPRequestBuilder* builder) {
    memset(builder, 0, sizeof(HTTPRequestBuilder));
    strcpy(builder->request.method, "GET");
}

void builder_setMethod(HTTPRequestBuilder* builder, const char* method) {
    strncpy(builder->request.method, method, sizeof(builder->request.method) - 1);
}

void builder_setURL(HTTPRequestBuilder* builder, const char* url) {
    strncpy(builder->request.url, url, sizeof(builder->request.url) - 1);
}

void builder_addHeader(HTTPRequestBuilder* builder, const char* key, const char* value) {
    if (builder->request.headerCount >= MAX_HEADERS) {
        printf("Header limit reached, dropping: %s\n", key);
        return;
    }
    KeyValue* h = &builder->request.headers[builder->request.headerCount++];
    strncpy(h->key, key, sizeof(h->key) - 1);
    strncpy(h->value, value, sizeof(h->value) - 1);
}

void builder_addQueryParam(HTTPRequestBuilder* builder, const char* key, const char* value) {
    if (builder->request.paramCount >= MAX_PARAMS) {
        printf("Param limit reached, dropping: %s\n", key);
        return;
    }
    KeyValue* p = &builder->request.queryParams[builder->request.paramCount++];
    strncpy(p->key, key, sizeof(p->key) - 1);
    strncpy(p->value, value, sizeof(p->value) - 1);
}

void builder_setBody(HTTPRequestBuilder* builder, const char* body) {
    strncpy(builder->request.body, body, sizeof(builder->request.body) - 1);
    builder->request.hasBody = true;
}

bool builder_build(HTTPRequestBuilder* builder, HTTPRequest* out) {
    if (strlen(builder->request.url) == 0) {
        printf("Build failed: URL is required\n");
        return false;
    }
    *out = builder->request;
    return true;
}

int main(void) {
    HTTPRequestBuilder builder;
    builder_init(&builder);

    builder_setMethod(&builder, "POST");
    builder_setURL(&builder, "https://api.example.com/users");
    builder_addHeader(&builder, "Content-Type", "application/json");
    builder_addHeader(&builder, "Authorization", "Bearer token123");
    builder_addQueryParam(&builder, "verbose", "true");
    builder_setBody(&builder, "{\"name\":\"Anuraag\"}");

    HTTPRequest request;
    if (builder_build(&builder, &request)) {
        printf("Method: %s\n", request.method);
        printf("URL: %s\n", request.url);
        printf("Headers:\n");
        for (int i = 0; i < request.headerCount; i++) {
            printf("  %s: %s\n", request.headers[i].key, request.headers[i].value);
        }
        printf("Query Params:\n");
        for (int i = 0; i < request.paramCount; i++) {
            printf("  %s: %s\n", request.queryParams[i].key, request.queryParams[i].value);
        }
        printf("Body: %s\n", request.hasBody ? request.body : "none");
    } else {
        printf("Request build failed\n");
    }

    // Invalid case: no URL set
    HTTPRequestBuilder badBuilder;
    builder_init(&badBuilder);
    builder_setMethod(&badBuilder, "GET");

    HTTPRequest badRequest;
    bool ok = builder_build(&badBuilder, &badRequest);
    printf(ok ? "Unexpected success\n" : "Bad request correctly returned nil\n");

    return 0;
}

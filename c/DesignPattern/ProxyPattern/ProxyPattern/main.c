//
//  main.c
//  ProxyPattern
//
//  Created by Anussha on 09/09/26.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct DatabaseConnection {
    const char* (*query)(struct DatabaseConnection* self, const char* sql);
    void* data;
} DatabaseConnection;

typedef struct {
    int connected; // just a marker, no real fields needed
} RealDatabaseConnection;

const char* realQuery(DatabaseConnection* self, const char* sql) {
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "Result of '%s'", sql);
    return buffer;
}

RealDatabaseConnection* createRealConnection(void) {
    printf("Connecting to database... (expensive setup)\n");
    sleep(1); // simulate expensive work (1 second, since C has no easy millisecond sleep without extra headers)
    printf("Connected.\n");
    RealDatabaseConnection* conn = malloc(sizeof(RealDatabaseConnection));
    conn->connected = 1;
    return conn;
}

typedef struct {
    RealDatabaseConnection* real; // NULL until first query
} ProxyData;

const char* proxyQuery(DatabaseConnection* self, const char* sql) {
    ProxyData* pd = (ProxyData*)self->data;
    if (pd->real == NULL) {
        printf("Proxy: no real connection yet, creating one now...\n");
        pd->real = createRealConnection();
    }
    DatabaseConnection realConn = { realQuery, pd->real };
    return realConn.query(&realConn, sql);
}

DatabaseConnection makeProxy(ProxyData* storage) {
    storage->real = NULL;
    DatabaseConnection proxy = { proxyQuery, storage };
    return proxy;
}

int main(void) {
    printf("Creating proxy...\n");
    ProxyData storage;
    DatabaseConnection proxyDb = makeProxy(&storage);
    printf("Proxy created (no real connection yet)\n");

    printf("%s\n", proxyDb.query(&proxyDb, "SELECT * FROM users"));
    printf("%s\n", proxyDb.query(&proxyDb, "SELECT * FROM orders"));

    free(storage.real);
    return 0;
}

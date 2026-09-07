//
//  main.c
//  DecoratorPattern
//
//  Created by Anussha on 07/09/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct DataFetcher {
    char* (*fetch)(struct DataFetcher* self);
    void* data;   // opaque pointer to whatever this fetcher needs (payload, or a wrapped fetcher)
} DataFetcher;

// --- BasicFetcher ---
char* basicFetch(DataFetcher* self) {
    return (char*)self->data;   // data IS the payload string directly
}

DataFetcher makeBasicFetcher(const char* payload) {
    DataFetcher f;
    f.fetch = basicFetch;
    f.data = (void*)payload;
    return f;
}

// --- LoggingFetcher ---
char* loggingFetch(DataFetcher* self) {
    DataFetcher* wrapped = (DataFetcher*)self->data;
    printf("Fetching...\n");
    char* result = wrapped->fetch(wrapped);
    printf("Fetched: %s\n", result);
    return result;
}

DataFetcher makeLoggingFetcher(DataFetcher* wrapped) {
    DataFetcher f;
    f.fetch = loggingFetch;
    f.data = (void*)wrapped;
    return f;
}

typedef struct {
    DataFetcher* wrapped;
    char* cachedResult;   // NULL means "not cached yet"
} CachingData;

char* cachingFetch(DataFetcher* self) {
    CachingData* cd = (CachingData*)self->data;
    if (cd->cachedResult != NULL) {
        printf("Returning cached result\n");
        return cd->cachedResult;
    }
    char* result = cd->wrapped->fetch(cd->wrapped);
    cd->cachedResult = result;
    return result;
}

DataFetcher makeCachingFetcher(DataFetcher* wrapped, CachingData* storage) {
    storage->wrapped = wrapped;
    storage->cachedResult = NULL;
    DataFetcher f;
    f.fetch = cachingFetch;
    f.data = (void*)storage;
    return f;
}

int main(void) {
    DataFetcher basic = makeBasicFetcher("user data");
    CachingData cachingStorage;
    DataFetcher caching = makeCachingFetcher(&basic, &cachingStorage);
    DataFetcher logging = makeLoggingFetcher(&caching);

    printf("%s\n", logging.fetch(&logging));
    printf("---\n");
    printf("%s\n", logging.fetch(&logging));
    return 0;
}

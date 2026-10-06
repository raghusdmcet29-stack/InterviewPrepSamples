//
//  main.c
//  ReaderWriterLock
//
//  Created by Anussha on 06/10/26.
//
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    char theme[16];
    pthread_rwlock_t rw;
} Settings;

Settings settings = {
    .theme = "light",
    .rw = PTHREAD_RWLOCK_INITIALIZER
};

void readTheme(int reader, char *out, size_t outSize) {
    pthread_rwlock_rdlock(&settings.rw);
    printf("Reader %d started\n", reader);
    sleep(1);
    snprintf(out, outSize, "%s", settings.theme);
    printf("Reader %d finished\n", reader);
    pthread_rwlock_unlock(&settings.rw);
}

void writeTheme(const char *newTheme) {
    pthread_rwlock_wrlock(&settings.rw);
    printf("Writer started\n");
    sleep(1);
    snprintf(settings.theme, sizeof(settings.theme), "%s", newTheme);
    printf("Writer finished\n");
    pthread_rwlock_unlock(&settings.rw);
}

void *readerTask(void *arg) {
    int reader = *(int *)arg;
    char value[16];
    readTheme(reader, value, sizeof(value));
    printf("Reader %d saw %s\n", reader, value);
    return NULL;
}

void *writerTask(void *arg) {
    writeTheme("dark");
    return NULL;
}

double nowSeconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int main(void) {
    double start = nowSeconds();

    int id1 = 1;
    int id2 = 2;
    int id3 = 3;
    pthread_t r1, r2, r3, w;

    pthread_create(&r1, NULL, readerTask, &id1);
    pthread_create(&r2, NULL, readerTask, &id2);
    usleep(200000);
    pthread_create(&w, NULL, writerTask, NULL);
    usleep(200000);
    pthread_create(&r3, NULL, readerTask, &id3);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w, NULL);
    pthread_join(r3, NULL);

    printf("Done in %.1f seconds\n", nowSeconds() - start);
    return 0;
}






/*
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    char theme[16];
    pthread_rwlock_t rw;
} Settings;

Settings settings = {
    .theme = "light",
    .rw = PTHREAD_RWLOCK_INITIALIZER
};

void readTheme(int reader, char *out, size_t outSize) {
    pthread_rwlock_rdlock(&settings.rw);
    printf("Reader %d started\n", reader);
    sleep(1);
    snprintf(out, outSize, "%s", settings.theme);
    printf("Reader %d finished\n", reader);
    pthread_rwlock_unlock(&settings.rw);
}

void *readerTask(void *arg) {
    int reader = *(int *)arg;
    char value[16];
    readTheme(reader, value, sizeof(value));
    return NULL;
}

double nowSeconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int main(void) {
    double start = nowSeconds();

    pthread_t readers[3];
    int ids[3] = {1, 2, 3};
    for (int r = 0; r < 3; r++) {
        pthread_create(&readers[r], NULL, readerTask, &ids[r]);
    }
    for (int r = 0; r < 3; r++) {
        pthread_join(readers[r], NULL);
    }

    printf("Done in %.1f seconds\n", nowSeconds() - start);
    return 0;
}
*/

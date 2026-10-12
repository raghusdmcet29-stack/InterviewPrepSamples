//
//  main.c
//  Concurrency12-Barrier
//
//  Created by Anussha on 12/10/26.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    pthread_mutex_t m;
    pthread_cond_t cv;
    int total;
    int arrived;
} Barrier;

static pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

void printLine(const char *text) {
    pthread_mutex_lock(&printMutex);
    printf("%s\n", text);
    pthread_mutex_unlock(&printMutex);
}

void barrierInit(Barrier *b, int total) {
    pthread_mutex_init(&b->m, NULL);
    pthread_cond_init(&b->cv, NULL);
    b->total = total;
    b->arrived = 0;
}

void barrierWait(Barrier *b) {
    pthread_mutex_lock(&b->m);
    b->arrived++;
    if (b->arrived == b->total) {
        pthread_cond_broadcast(&b->cv);
    } else {
        while (b->arrived < b->total) {
            pthread_cond_wait(&b->cv, &b->m);
        }
    }
    pthread_mutex_unlock(&b->m);
}

typedef struct {
    const char *name;
    unsigned int seconds;
    Barrier *barrier;
} WorkerArgs;

void *worker(void *arg) {
    WorkerArgs *w = (WorkerArgs *)arg;
    char buf[64];

    sleep(w->seconds);
    snprintf(buf, sizeof buf, "%s arrived at barrier", w->name);
    printLine(buf);

    barrierWait(w->barrier);

    snprintf(buf, sizeof buf, "%s passed the barrier", w->name);
    printLine(buf);
    return NULL;
}

int main(void) {
    Barrier barrier;
    barrierInit(&barrier, 3);

    WorkerArgs argsA = {"A", 1, &barrier};
    WorkerArgs argsB = {"B", 2, &barrier};
    WorkerArgs argsC = {"C", 3, &barrier};

    pthread_t a, b, c;
    pthread_create(&a, NULL, worker, &argsA);
    pthread_create(&b, NULL, worker, &argsB);
    pthread_create(&c, NULL, worker, &argsC);

    pthread_join(a, NULL);
    pthread_join(b, NULL);
    pthread_join(c, NULL);

    printLine("Done");
    return 0;
}

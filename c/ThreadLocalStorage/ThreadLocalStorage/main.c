//
//  main.c
//  ThreadLocalStorage
//
//  Created by Anussha on 12/10/26.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

_Thread_local const char *threadName;

static pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

void printLine(const char *text) {
    pthread_mutex_lock(&printMutex);
    printf("%s\n", text);
    pthread_mutex_unlock(&printMutex);
}

void *work(void *arg) {
    threadName = (const char *)arg;
    sleep(1);
    char buf[64];
    snprintf(buf, sizeof buf, "%s reads %s", (const char *)arg, threadName);
    printLine(buf);
    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, work, "T1");
    pthread_create(&t2, NULL, work, "T2");
    pthread_create(&t3, NULL, work, "T3");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    printLine("Done");
    return 0;
}

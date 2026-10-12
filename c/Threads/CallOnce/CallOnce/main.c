//
//  main.c
//  CallOnce
//
//  Created by Anussha on 12/10/26.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static pthread_once_t configOnce = PTHREAD_ONCE_INIT;
static const char *config = NULL;

static void loadConfig(void) {
    printf("loading config\n");
    sleep(1);
    config = "settings";
}

static const char *getConfig(const char *who) {
    pthread_once(&configOnce, loadConfig);
    printf("%s got %s\n", who, config);
    return config;
}

static void *worker(void *arg) {
    getConfig((const char *)arg);
    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, worker, "T1");
    pthread_create(&t2, NULL, worker, "T2");
    pthread_create(&t3, NULL, worker, "T3");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    printf("Done\n");
    return 0;
}

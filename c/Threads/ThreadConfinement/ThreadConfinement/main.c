//
//  main.c
//  ThreadConfinement
//
//  Created by Anussha on 13/10/26.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
int tasks[16];
int head = 0;
int tail = 0;

void *worker(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&m);
        while (head == tail) {
            pthread_cond_wait(&cv, &m);
        }
        int task = tasks[head];
        head++;
        pthread_mutex_unlock(&m);

        if (task == -1) {
            break;
        }
        printf("Running task %d\n", task);
    }
    return NULL;
}

void submit(int task) {
    pthread_mutex_lock(&m);
    tasks[tail] = task;
    tail++;
    pthread_mutex_unlock(&m);
    pthread_cond_signal(&cv);
}

int main(void) {
    pthread_t t;
    pthread_create(&t, NULL, worker, NULL);

    submit(1);
    submit(2);
    sleep(2);
    submit(3);
    submit(-1);

    pthread_join(t, NULL);
    printf("Done\n");
    return 0;
}


/*
#include <pthread.h>
#include <stdio.h>

int tasks[3] = {1, 2, 3};
int head = 0;
int count = 3;

void *worker(void *arg) {
    (void)arg;
    while (head < count) {
        int task = tasks[head];
        head++;
        printf("Running task %d\n", task);
    }
    return NULL;
}

int main(void) {
    pthread_t t;
    pthread_create(&t, NULL, worker, NULL);
    pthread_join(t, NULL);
    printf("Done\n");
    return 0;
}
*/

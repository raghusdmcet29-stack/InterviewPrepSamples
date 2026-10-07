//
//  main.c
//  DiningPhilosophers
//
//  Created by Anussha on 07/10/26.
//
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t forks[3] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};
// avoiding dead lock
void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % 3;
    int first = left < right ? left : right;
    int second = left < right ? right : left;

    pthread_mutex_lock(&forks[first]);
    printf("P%d picked up F%d\n", id, first);
    sleep(1);
    printf("P%d wants F%d\n", id, second);
    pthread_mutex_lock(&forks[second]);
    printf("P%d eating\n", id);
    pthread_mutex_unlock(&forks[second]);
    pthread_mutex_unlock(&forks[first]);
    return NULL;
}


// dead lock
/*void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % 3;

    pthread_mutex_lock(&forks[left]);
    printf("P%d picked up F%d\n", id, left);
    sleep(1);
    printf("P%d wants F%d\n", id, right);
    pthread_mutex_lock(&forks[right]);
    printf("P%d eating\n", id);
    pthread_mutex_unlock(&forks[right]);
    pthread_mutex_unlock(&forks[left]);
    return NULL;
}*/

int main(void) {
    pthread_t threads[3];
    int ids[3] = {0, 1, 2};

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Done\n");
    return 0;
}

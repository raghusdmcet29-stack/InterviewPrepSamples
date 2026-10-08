//
//  main.c
//  Semaphore
//
//  Created by Anussha on 08/10/26.
//

#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t *carPark;

void *car(void *arg) {
    char *name = (char *)arg;
    sem_wait(carPark);
    printf("Car %s parked\n", name);
    sleep(2);
    printf("Car %s left\n", name);
    sem_post(carPark);
    return NULL;
}

int main(void) {
    sem_unlink("/carpark");
    carPark = sem_open("/carpark", O_CREAT, 0644, 2);
    if (carPark == SEM_FAILED) {
        printf("sem_open failed\n");
        return 1;
    }

    char *names[3] = {"A", "B", "C"};
    pthread_t cars[3];

    for (int i = 0; i < 3; i++) {
        pthread_create(&cars[i], NULL, car, names[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(cars[i], NULL);
    }

    printf("Done\n");
    sem_close(carPark);
    sem_unlink("/carpark");
    return 0;
}



/*
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>

int main(void) {
    sem_unlink("/carpark");
    sem_t *carPark = sem_open("/carpark", O_CREAT, 0644, 2);
    if (carPark == SEM_FAILED) {
        printf("sem_open failed\n");
        return 1;
    }

    sem_wait(carPark);
    printf("Car A parked\n");

    sem_wait(carPark);
    printf("Car B parked\n");

    sem_close(carPark);
    sem_unlink("/carpark");
    return 0;
}
*/

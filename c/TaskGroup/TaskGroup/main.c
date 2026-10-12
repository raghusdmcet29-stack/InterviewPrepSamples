//
//  main.c
//  TaskGroup
//
//  Created by Anussha on 12/10/26.
//
// sucess and failure jobs

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    const char *name;
    unsigned int seconds;
    int fail;
    int ok;
} Job;

void *runJob(void *arg) {
    Job *j = (Job *)arg;
    sleep(j->seconds);
    if (j->fail) {
        printf("%s failed\n", j->name);
        j->ok = 0;
    } else {
        printf("%s finished\n", j->name);
        j->ok = 1;
    }
    return NULL;
}

int main(void) {
    Job jobs[3] = {
        {"J1", 3, 0, 0},
        {"J2", 1, 1, 0},
        {"J3", 2, 0, 0}
    };

    pthread_t threads[3];
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, runJob, &jobs[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < 3; i++) {
        printf("%s ok: %d\n", jobs[i].name, jobs[i].ok);
    }
    return 0;
}



// only sucess jobs
/*#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    const char *name;
    unsigned int seconds;
    const char *result;
} Job;

void *runJob(void *arg) {
    Job *j = (Job *)arg;
    sleep(j->seconds);
    printf("%s finished\n", j->name);
    j->result = j->name;
    return NULL;
}

int main(void) {
    Job jobs[3] = {
        {"J1", 3, NULL},
        {"J2", 1, NULL},
        {"J3", 2, NULL}
    };

    pthread_t threads[3];
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, runJob, &jobs[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Results: ");
    for (int i = 0; i < 3; i++) {
        printf("%s ", jobs[i].result);
    }
    printf("\n");
    return 0;
}
*/

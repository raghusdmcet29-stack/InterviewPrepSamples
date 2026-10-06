//
//  main.c
//  ThreadPool
//
//  Created by Anussha on 06/10/26.
//

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_JOBS 16

typedef struct {
    int id;
} Job;

void runJob(Job job) {
    printf("Job %d started\n", job.id);
    sleep(1);
    printf("Job %d finished\n", job.id);
}

typedef struct {
    Job jobs[MAX_JOBS];
    int head;
    int tail;
    bool closed;
    pthread_mutex_t m;
    pthread_cond_t cv;
} JobQueue;

JobQueue queue = {
    .head = 0,
    .tail = 0,
    .closed = false,
    .m = PTHREAD_MUTEX_INITIALIZER,
    .cv = PTHREAD_COND_INITIALIZER
};

void queueAdd(Job job) {
    pthread_mutex_lock(&queue.m);
    queue.jobs[queue.tail] = job;
    queue.tail++;
    pthread_cond_signal(&queue.cv);
    pthread_mutex_unlock(&queue.m);
}

void queueClose(void) {
    pthread_mutex_lock(&queue.m);
    queue.closed = true;
    pthread_cond_broadcast(&queue.cv);
    pthread_mutex_unlock(&queue.m);
}

bool queueTake(Job *out) {
    pthread_mutex_lock(&queue.m);
    while (queue.head == queue.tail && !queue.closed) {
        pthread_cond_wait(&queue.cv, &queue.m);
    }
    if (queue.head == queue.tail) {
        pthread_mutex_unlock(&queue.m);
        return false;
    }
    *out = queue.jobs[queue.head];
    queue.head++;
    pthread_mutex_unlock(&queue.m);
    return true;
}

void *workerLoop(void *arg) {
    int w = *(int *)arg;
    Job job;
    while (queueTake(&job)) {
        runJob(job);
    }
    printf("Worker %d quitting\n", w);
    return NULL;
}

int main(void) {
    for (int id = 1; id <= 10; id++) {
        Job job = {id};
        queueAdd(job);
    }
    queueClose();

    pthread_t workers[3];
    int ids[3] = {1, 2, 3};
    for (int w = 0; w < 3; w++) {
        pthread_create(&workers[w], NULL, workerLoop, &ids[w]);
    }
    for (int w = 0; w < 3; w++) {
        pthread_join(workers[w], NULL);
    }
    printf("Done\n");
    return 0;
}


/*
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_JOBS 16

typedef struct {
    int id;
} Job;

void runJob(Job job) {
    printf("Job %d started\n", job.id);
    sleep(1);
    printf("Job %d finished\n", job.id);
}

typedef struct {
    Job jobs[MAX_JOBS];
    int head;
    int tail;
    bool closed;
    pthread_mutex_t m;
    pthread_cond_t cv;
} JobQueue;

JobQueue queue = {
    .head = 0,
    .tail = 0,
    .closed = false,
    .m = PTHREAD_MUTEX_INITIALIZER,
    .cv = PTHREAD_COND_INITIALIZER
};

void queueAdd(Job job) {
    pthread_mutex_lock(&queue.m);
    queue.jobs[queue.tail] = job;
    queue.tail++;
    pthread_cond_signal(&queue.cv);
    pthread_mutex_unlock(&queue.m);
}

void queueClose(void) {
    pthread_mutex_lock(&queue.m);
    queue.closed = true;
    pthread_cond_broadcast(&queue.cv);
    pthread_mutex_unlock(&queue.m);
}

bool queueTake(Job *out) {
    pthread_mutex_lock(&queue.m);
    while (queue.head == queue.tail && !queue.closed) {
        pthread_cond_wait(&queue.cv, &queue.m);
    }
    if (queue.head == queue.tail) {
        pthread_mutex_unlock(&queue.m);
        return false;
    }
    *out = queue.jobs[queue.head];
    queue.head++;
    pthread_mutex_unlock(&queue.m);
    return true;
}

void *workerLoop(void *arg) {
    Job job;
    while (queueTake(&job)) {
        runJob(job);
    }
    printf("Worker quitting\n");
    return NULL;
}

int main(void) {
    Job j1 = {1};
    Job j2 = {2};
    Job j3 = {3};
    queueAdd(j1);
    queueAdd(j2);
    queueAdd(j3);
    queueClose();

    pthread_t worker;
    pthread_create(&worker, NULL, workerLoop, NULL);
    pthread_join(worker, NULL);
    printf("Done\n");
    return 0;
}
*/
// worker thread running paralle jobs
/*
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_JOBS 16

typedef struct {
    int id;
} Job;

void runJob(Job job) {
    printf("Job %d started\n", job.id);
    sleep(1);
    printf("Job %d finished\n", job.id);
}

typedef struct {
    Job jobs[MAX_JOBS];
    int head;
    int tail;
    pthread_mutex_t m;
    pthread_cond_t cv;
} JobQueue;

JobQueue queue = {
    .head = 0,
    .tail = 0,
    .m = PTHREAD_MUTEX_INITIALIZER,
    .cv = PTHREAD_COND_INITIALIZER
};

void queueAdd(Job job) {
    pthread_mutex_lock(&queue.m);
    queue.jobs[queue.tail] = job;
    queue.tail++;
    pthread_cond_signal(&queue.cv);
    pthread_mutex_unlock(&queue.m);
}

Job queueTake(void) {
    pthread_mutex_lock(&queue.m);
    while (queue.head == queue.tail) {
        pthread_cond_wait(&queue.cv, &queue.m);
    }
    Job job = queue.jobs[queue.head];
    queue.head++;
    pthread_mutex_unlock(&queue.m);
    return job;
}

void *workerLoop(void *arg) {
    for (int i = 0; i < 2; i++) {
        Job job = queueTake();
        runJob(job);
    }
    return NULL;
}

int main(void) {
    for (int id = 1; id <= 6; id++) {
        Job job = {id};
        queueAdd(job);
    }

    pthread_t workers[3];
    for (int w = 0; w < 3; w++) {
        pthread_create(&workers[w], NULL, workerLoop, NULL);
    }
    for (int w = 0; w < 3; w++) {
        pthread_join(workers[w], NULL);
    }
    printf("Done\n");
    return 0;
}
*/
/*
 // instead of using main thread use separate worker thread
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_JOBS 16

typedef struct {
    int id;
} Job;

void runJob(Job job) {
    printf("Job %d started\n", job.id);
    sleep(1);
    printf("Job %d finished\n", job.id);
}

typedef struct {
    Job jobs[MAX_JOBS];
    int head;
    int tail;
    pthread_mutex_t m;
    pthread_cond_t cv;
} JobQueue;

JobQueue queue = {
    .head = 0,
    .tail = 0,
    .m = PTHREAD_MUTEX_INITIALIZER,
    .cv = PTHREAD_COND_INITIALIZER
};

void queueAdd(Job job) {
    pthread_mutex_lock(&queue.m);
    queue.jobs[queue.tail] = job;
    queue.tail++;
    pthread_cond_signal(&queue.cv);
    pthread_mutex_unlock(&queue.m);
}

Job queueTake(void) {
    pthread_mutex_lock(&queue.m);
    while (queue.head == queue.tail) {
        pthread_cond_wait(&queue.cv, &queue.m);
    }
    Job job = queue.jobs[queue.head];
    queue.head++;
    pthread_mutex_unlock(&queue.m);
    return job;
}

void *workerLoop(void *arg) {
    for (int i = 0; i < 3; i++) {
        Job job = queueTake();
        runJob(job);
    }
    return NULL;
}

int main(void) {
    Job j1 = {1};
    Job j2 = {2};
    Job j3 = {3};
    queueAdd(j1);
    queueAdd(j2);
    queueAdd(j3);

    pthread_t worker;
    pthread_create(&worker, NULL, workerLoop, NULL);
    pthread_join(worker, NULL);
    printf("Done\n");
    return 0;
}*/
/*
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_JOBS 16

typedef struct {
    int id;
} Job;

void runJob(Job job) {
    printf("Job %d started\n", job.id);
    sleep(1);
    printf("Job %d finished\n", job.id);
}

typedef struct {
    Job jobs[MAX_JOBS];
    int head;
    int tail;
    pthread_mutex_t m;
    pthread_cond_t cv;
} JobQueue;

JobQueue queue = {
    .head = 0,
    .tail = 0,
    .m = PTHREAD_MUTEX_INITIALIZER,
    .cv = PTHREAD_COND_INITIALIZER
};

void queueAdd(Job job) {
    pthread_mutex_lock(&queue.m);
    queue.jobs[queue.tail] = job;
    queue.tail++;
    pthread_cond_signal(&queue.cv);
    pthread_mutex_unlock(&queue.m);
}

Job queueTake(void) {
    pthread_mutex_lock(&queue.m);
    while (queue.head == queue.tail) {
        pthread_cond_wait(&queue.cv, &queue.m);
    }
    Job job = queue.jobs[queue.head];
    queue.head++;
    pthread_mutex_unlock(&queue.m);
    return job;
}

int main(void) {
    Job j1 = {1};
    Job j2 = {2};
    Job j3 = {3};
    queueAdd(j1);
    queueAdd(j2);
    queueAdd(j3);

    for (int i = 0; i < 3; i++) {
        Job job = queueTake();
        runJob(job);
    }
    printf("Done\n");
    return 0;
}

*/

/*
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int id;
} Job;

void runJob(Job job) {
    printf("Job %d started\n", job.id);
    sleep(1);
    printf("Job %d finished\n", job.id);
}

int main(void) {
    Job job = {1};
    runJob(job);
    printf("Done\n");
    return 0;
}
*/

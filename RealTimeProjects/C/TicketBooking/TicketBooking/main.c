//
//  main.c
//  TicketBooking
//
//  Created by Anussha on 15/10/26.
//

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>

static pthread_mutex_t print_m = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t seats_m = PTHREAD_MUTEX_INITIALIZER;
static bool seats[3] = {false, false, false};   // false = free, true = booked

// Barrier: each thread waits until `total` threads have arrived.
static pthread_mutex_t gate_m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gate_cv = PTHREAD_COND_INITIALIZER;
static int arrived = 0;
static const int total = 3;

// Compare-and-swap counter: adds 1 only if the value is still what we read.
static atomic_int bookings = 0;

static void increment(atomic_int *counter) {
    int old = atomic_load(counter);
    while (!atomic_compare_exchange_strong(counter, &old, old + 1)) {
        // on failure, `old` now holds the current value, so we just retry
    }
}

static void waitAtGate(void) {
    pthread_mutex_lock(&gate_m);
    arrived++;
    if (arrived == total) {
        pthread_cond_broadcast(&gate_cv);   // last one in opens the gate
    }else {
        while (arrived < total) {
            pthread_cond_wait(&gate_cv, &gate_m);
        }
    }
    pthread_mutex_unlock(&gate_m);
}

// Thread-local storage: each thread keeps its own receipt number.
static _Thread_local int receipt = 0;

typedef struct {
    int customer;
    int receipt;
    bool booked;
} BookingResult;


static void *bookSeat(void *arg) {
    BookingResult *result = (BookingResult *)arg;
    int customer = *(int *)arg;
    receipt = 100 + customer;
    
    waitAtGate();
    
    bool booked = false;
    pthread_mutex_lock(&seats_m);
    if (!seats[customer - 1]) {
        seats[customer - 1] = true;
        booked = true;
        increment(&bookings);
    }
    pthread_mutex_unlock(&seats_m);
    
    // Each thread writes only into its own slot.
    result->receipt = receipt;
    result->booked = booked;
    return NULL;
}

int main(void) {
    pthread_t threads[3];
    BookingResult results[3];

    for (int i = 0; i < total; i++) {
        results[i].customer = i + 1;
        results[i].receipt = 0;
        results[i].booked = false;
        pthread_create(&threads[i], NULL, bookSeat, &results[i]);
    }
    
    // Collect every result, then print the summary.
    for (int i = 0; i < total; i++) {
        pthread_join(threads[i], NULL);
    }
    
    for (int i = 0; i < total; i++) {
        printf("customer %d: receipt %d, booked: %s\n",
               results[i].customer,
               results[i].receipt,
               results[i].booked ? "true" : "false");
    }
    
    printf("seats: %s %s %s\n",
               seats[0] ? "true" : "false",
               seats[1] ? "true" : "false",
               seats[2] ? "true" : "false");
        printf("total bookings: %d\n", atomic_load(&bookings));
    
    return 0;
}

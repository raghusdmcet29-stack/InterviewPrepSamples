//
//  main.c
//  ObserverPattern
//
//  Created by Anussha on 03/09/26.
//

#include <stdio.h>
#define MAX_OBSERVERS 10

typedef struct Observer {
    void (*update)(struct Observer* self, int newValue);
    const char* name;
} Observer;

typedef struct Subject {
    Observer* observers[MAX_OBSERVERS];
    int count;
} Subject;


void consoleLoggerUpdate(Observer* self, int newValue) {
    printf("%s: value changed to %d\n", self->name, newValue);
}

void alertObserverUpdate(Observer* self, int newValue) {
    if (newValue > 100) {
        printf("%s: ALERT! Value %d exceeds threshold!\n", self->name, newValue);
    } else {
        printf("%s: value %d is within normal range.\n", self->name, newValue);
    }
}


void subjectAttach(Subject* subject, Observer* observer) {
    subject->observers[subject->count] = observer;
    subject->count++;
}

void subjectNotifyAll(Subject* subject, int newValue) {
    for (int i = 0; i < subject->count; i++) {
        Observer* observer = subject->observers[i];
        observer->update(observer, newValue);
    }
}

int main(void) {
    Observer logger = { consoleLoggerUpdate, "ConsoleLogger" };
    Observer alert = { alertObserverUpdate, "AlertObserver" };

    Subject subject = { .count = 0 };
    subjectAttach(&subject, &logger);
    subjectAttach(&subject, &alert);

    subjectNotifyAll(&subject, 150);
    // ConsoleLogger: value changed to 150
    // AlertObserver: ALERT! Value 150 exceeds threshold!

    subjectNotifyAll(&subject, 50);
    // ConsoleLogger: value changed to 50
    // AlertObserver: value 50 is within normal range.

    return 0;
}



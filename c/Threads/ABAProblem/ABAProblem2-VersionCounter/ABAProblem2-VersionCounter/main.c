//
//  main.c
//  ABAProblem2-VersionCounter
//
//  Created by Anussha on 14/10/26.
//
//C code (ABAProblem2-VersionCounter, C part 5 (2 thread with atomic)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *node;
    uint64_t version;   // 64-bit so there is no padding
} TaggedTop;

typedef struct {
    _Atomic TaggedTop top;
} VersionedStack;

Node *make_node(int value, Node *next) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

int pop(VersionedStack *s, int *out) {
    TaggedTop current = atomic_load(&s->top);
    while (1) {
        if (current.node == NULL) {
            return 0;  // empty stack
        }
        TaggedTop next = { current.node->next, current.version + 1 };
        // On failure, current is updated to the value now in top
        if (atomic_compare_exchange_weak(&s->top, &current, next)) {
            *out = current.node->value;
            return 1;
        }
    }
}

typedef struct {
    VersionedStack *stack;
    int *result_slot;
} Job;

void *worker(void *arg) {
    Job *job = arg;
    int value = -1;
    int popped = pop(job->stack, &value);
    *job->result_slot = popped ? value : -1;
    return NULL;
}

int main(void) {
    Node *b = make_node(2, NULL);
    Node *a = make_node(1, b);

    VersionedStack stack;
    atomic_init(&stack.top, (TaggedTop){ a, 0 });

    printf("lock-free: %d\n", atomic_is_lock_free(&stack.top));

    int results[2];
    Job jobs[2];
    pthread_t threads[2];

    for (int i = 0; i < 2; i++) {
        jobs[i] = (Job){ &stack, &results[i] };
        pthread_create(&threads[i], NULL, worker, &jobs[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("thread 0 popped: %d\n", results[0]);
    printf("thread 1 popped: %d\n", results[1]);
    TaggedTop final_state = atomic_load(&stack.top);
    printf("top node value: %d\n", final_state.node ? final_state.node->value : -1);
    printf("top version: %llu\n", (unsigned long long)final_state.version);

    free(a);
    free(b);
    return 0;
}





//C code (ABAProblem2-VersionCounter, C part 4 (2 thread with mutex)
/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *node;
    uint64_t version;
} TaggedTop;

typedef struct {
    TaggedTop top;
    pthread_mutex_t m;
} VersionedStack;

Node *make_node(int value, Node *next) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

TaggedTop load(VersionedStack *s) {
    pthread_mutex_lock(&s->m);
    TaggedTop t = s->top;
    pthread_mutex_unlock(&s->m);
    return t;
}

int compare_and_swap(VersionedStack *s, TaggedTop expected, TaggedTop new_top) {
    pthread_mutex_lock(&s->m);
    int ok = (s->top.node == expected.node && s->top.version == expected.version);
    if (ok) {
        s->top = new_top;
    }
    pthread_mutex_unlock(&s->m);
    return ok;
}

int pop(VersionedStack *s, int *out) {
    while (1) {
        TaggedTop current = load(s);
        if (current.node == NULL) {
            return 0;
        }
        TaggedTop next = { current.node->next, current.version + 1 };
        if (compare_and_swap(s, current, next)) {
            *out = current.node->value;
            return 1;
        }
    }
}

typedef struct {
    VersionedStack *stack;
    int *result_slot;
} Job;

void *worker(void *arg) {
    Job *job = arg;
    int value = -1;
    int popped = pop(job->stack, &value);
    *job->result_slot = popped ? value : -1;
    return NULL;
}

int main(void) {
    Node *b = make_node(2, NULL);
    Node *a = make_node(1, b);
    VersionedStack stack = { { a, 0 }, PTHREAD_MUTEX_INITIALIZER };

    int results[2];
    Job jobs[2];
    pthread_t threads[2];

    for (int i = 0; i < 2; i++) {
        jobs[i] = (Job){ &stack, &results[i] };
        pthread_create(&threads[i], NULL, worker, &jobs[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("thread 0 popped: %d\n", results[0]);
    printf("thread 1 popped: %d\n", results[1]);
    TaggedTop final_state = load(&stack);
    printf("top node value: %d\n", final_state.node ? final_state.node->value : -1);
    printf("top version: %llu\n", (unsigned long long)final_state.version);

    pthread_mutex_destroy(&stack.m);
    free(a);
    free(b);
    return 0;
}
/*
//ABAProblem2-VersionCounter, C part 3
/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *node;
    uint64_t version;
} TaggedTop;

typedef struct {
    TaggedTop top;
} VersionedStack;

Node *make_node(int value, Node *next) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

int compare_and_swap(VersionedStack *s, TaggedTop expected, TaggedTop new_top) {
    if (s->top.node == expected.node && s->top.version == expected.version) {
        s->top = new_top;
        return 1;
    }
    return 0;
}

// Pop with a retry loop: read fresh state, try the CAS, retry if it fails.
// Returns 1 if a value was popped (stored in *out), 0 if the stack was empty.
int pop(VersionedStack *s, int *out) {
    while (1) {
        TaggedTop current = s->top;
        if (current.node == NULL) {
            return 0;  // empty stack
        }
        TaggedTop next = { current.node->next, current.version + 1 };
        if (compare_and_swap(s, current, next)) {
            *out = current.node->value;
            return 1;
        }
        // CAS failed: state changed, loop and read again
    }
}

int main(void) {
    Node *b = make_node(2, NULL);
    Node *a = make_node(1, b);
    VersionedStack stack = { { a, 0 } };

    // Thread 2's work, done up front for this single-threaded test:
    // pop A, pop B, push A back (version goes up each change)
    stack.top = (TaggedTop){ b, 1 };
    stack.top = (TaggedTop){ NULL, 2 };
    a->next = NULL;
    stack.top = (TaggedTop){ a, 3 };

    // Thread 1 pops now, reading fresh state
    int value = -1;
    int popped = pop(&stack, &value);
    printf("popped value: %d\n", popped ? value : -1);
    printf("top node value: %d\n", stack.top.node ? stack.top.node->value : -1);
    printf("top version: %llu\n", (unsigned long long)stack.top.version);

    free(a);
    free(b);
    return 0;
}
*/

//ABAProblem2-VersionCounter, C part 2
/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *node;
    uint64_t version;
} TaggedTop;

typedef struct {
    TaggedTop top;
} VersionedStack;

Node *make_node(int value, Node *next) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

// Succeeds only if BOTH the node and the version match
int compare_and_swap(VersionedStack *s, TaggedTop expected, TaggedTop new_top) {
    if (s->top.node == expected.node && s->top.version == expected.version) {
        s->top = new_top;
        return 1;
    }
    return 0;
}

int main(void) {
    Node *b = make_node(2, NULL);
    Node *a = make_node(1, b);
    VersionedStack stack = { { a, 0 } };

    // Thread 1 saves the top and its next
    TaggedTop saved = stack.top;          // (A, 0)
    Node *saved_next = saved.node->next;  // B

    // Thread 2 pops A, pops B, pushes A back (version goes up each change)
    stack.top = (TaggedTop){ b, 1 };      // pop A
    stack.top = (TaggedTop){ NULL, 2 };   // pop B
    a->next = NULL;
    stack.top = (TaggedTop){ a, 3 };      // push A back

    // Thread 1 resumes and tries its CAS with the old value
    int succeeded = compare_and_swap(&stack, saved,
                                     (TaggedTop){ saved_next, saved.version + 1 });
    printf("CAS succeeded: %s\n", succeeded ? "true" : "false");
    printf("top node value: %d\n", stack.top.node->value);
    printf("top version: %llu\n", (unsigned long long)stack.top.version);

    free(a);
    free(b);
    return 0;
}
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

// The pointer and the version travel together as one value
typedef struct {
    Node *node;
    uint64_t version;   // 64-bit so there is no padding
} TaggedTop;

Node *make_node(int value, Node *next) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

int main(void) {
    Node *b = make_node(2, NULL);
    Node *a = make_node(1, b);

    TaggedTop start = { .node = a, .version = 0 };
    printf("node value: %d\n", start.node->value);
    printf("version: %llu\n", (unsigned long long)start.version);

    free(a);
    free(b);
    return 0;
}
*/

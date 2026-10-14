//
//  main.c
//  ABAProblem1-StaleNext
//
//  Created by Anussha on 14/10/26.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} Stack;

Node *make_node(int value, Node *next) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

// Simulated CAS: swap top only if it still equals expected
int compare_and_swap_top(Stack *s, Node *expected, Node *new_top) {
    if (s->top == expected) {
        s->top = new_top;
        return 1;
    }
    return 0;
}

int main(void) {
    Node *b = make_node(2, NULL);
    Node *a = make_node(1, b);
    Stack stack = { a };

    // Thread 1 reads the top and its next, then pauses
    Node *expected_top = stack.top;      // A
    Node *saved_next = expected_top->next; // B

    // Thread 2 pops A, pops B, pushes A back
    stack.top = b;
    stack.top = NULL;
    a->next = NULL;
    stack.top = a;

    // Thread 1 resumes and tries its CAS
    int succeeded = compare_and_swap_top(&stack, expected_top, saved_next);
    printf("CAS succeeded: %s\n", succeeded ? "true" : "false");
    printf("top value: %d\n", stack.top->value);

    free(a);
    free(b);
    return 0;
}




/*
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *make_node(int value, Node *next) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->next = next;
    return n;
}

int main(void) {
    Node *b = make_node(2, NULL);
    Node *a = make_node(1, b);

    printf("a value: %d\n", a->value);
    printf("a next value: %d\n", a->next->value);
    printf("a == b? %d\n", a == b);

    free(a);
    free(b);
    return 0;
}
*/

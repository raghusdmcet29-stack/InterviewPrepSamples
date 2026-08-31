//
//  main.c
//  DispatchTable
//
//  Created by Anussha on 31/08/26.
//

#include <stdio.h>
/*// switch based version
typedef enum { RED, GREEN, YELLOW, NUM_STATES } TrafficState;

const char* stateName(TrafficState s) {
    switch (s) {
        case RED: return "RED";
        case GREEN: return "GREEN";
        case YELLOW: return "YELLOW";
        default: return "UNKNOWN";
    }
}

// Naive: switch statement to decide next state
TrafficState nextStateNaive(TrafficState current) {
    switch (current) {
        case RED: return GREEN;
        case GREEN: return YELLOW;
        case YELLOW: return RED;
        default: return RED;
    }
}

int main(void) {
    TrafficState state = RED;
    for (int i = 0; i < 6; i++) {
        printf("State: %s\n", stateName(state));
        state = nextStateNaive(state);
    }
    return 0;
}
*/
// Dispatch table version

typedef enum { RED, GREEN, YELLOW, NUM_STATES } TrafficState;

const char* stateName(TrafficState s) {
    switch (s) {
        case RED: return "RED";
        case GREEN: return "GREEN";
        case YELLOW: return "YELLOW";
        default: return "UNKNOWN";
    }
}

// Each handler returns the NEXT state — one function per state
TrafficState handleRed(void)    { return GREEN; }
TrafficState handleGreen(void)  { return YELLOW; }
TrafficState handleYellow(void) { return RED; }

// The dispatch table: array of function pointers, indexed by state
typedef TrafficState (*StateHandler)(void);

StateHandler dispatchTable[NUM_STATES] = {
    [RED]    = handleRed,
    [GREEN]  = handleGreen,
    [YELLOW] = handleYellow
};

int main(void) {
    TrafficState state = RED;
    for (int i = 0; i < 6; i++) {
        printf("State: %s\n", stateName(state));
        state = dispatchTable[state]();   // no switch/if — direct indexed call
    }
    return 0;
}

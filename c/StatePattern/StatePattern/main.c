//
//  main.c
//  StatePattern
//
//  Created by Anussha on 07/09/26.
//

#include <stdio.h>

typedef enum {
    STATE_CLOSED,
    STATE_LISTEN,
    STATE_SYN_RECEIVED,
    STATE_ESTABLISHED
} StateId;

typedef struct ConnectionState {
    StateId id;
    const char* (*description)(void);
    struct ConnectionState (*open)(void);
    struct ConnectionState (*receiveSyn)(void);
    struct ConnectionState (*establish)(void);
    struct ConnectionState (*close)(void);
} ConnectionState;

ConnectionState makeClosedState(void);
ConnectionState makeListenState(void);
ConnectionState makeSynReceivedState(void);
ConnectionState makeEstablishedState(void);

// --- Closed ---
const char* closedDescription(void) { return "Closed"; }
ConnectionState closedOpen(void)       { return makeListenState(); }
ConnectionState closedReceiveSyn(void) { return makeClosedState(); }  // invalid, stay put
ConnectionState closedEstablish(void)  { return makeClosedState(); }
ConnectionState closedClose(void)      { return makeClosedState(); }

ConnectionState makeClosedState(void) {
    ConnectionState s;
    s.id = STATE_CLOSED;
    s.description = closedDescription;
    s.open = closedOpen;
    s.receiveSyn = closedReceiveSyn;
    s.establish = closedEstablish;
    s.close = closedClose;
    return s;
}

// --- Listen ---
const char* listenDescription(void) { return "Listen"; }
ConnectionState listenOpen(void)       { return makeListenState(); }
ConnectionState listenReceiveSyn(void) { return makeSynReceivedState(); }
ConnectionState listenEstablish(void)  { return makeListenState(); }
ConnectionState listenClose(void)      { return makeClosedState(); }

ConnectionState makeListenState(void) {
    ConnectionState s;
    s.id = STATE_LISTEN;
    s.description = listenDescription;
    s.open = listenOpen;
    s.receiveSyn = listenReceiveSyn;
    s.establish = listenEstablish;
    s.close = listenClose;
    return s;
}

// --- SynReceived ---
const char* synReceivedDescription(void) { return "SynReceived"; }
ConnectionState synReceivedOpen(void)       { return makeSynReceivedState(); }
ConnectionState synReceivedReceiveSyn(void) { return makeSynReceivedState(); }
ConnectionState synReceivedEstablish(void)  { return makeEstablishedState(); }
ConnectionState synReceivedClose(void)      { return makeClosedState(); }

ConnectionState makeSynReceivedState(void) {
    ConnectionState s;
    s.id = STATE_SYN_RECEIVED;
    s.description = synReceivedDescription;
    s.open = synReceivedOpen;
    s.receiveSyn = synReceivedReceiveSyn;
    s.establish = synReceivedEstablish;
    s.close = synReceivedClose;
    return s;
}

// --- Established ---
const char* establishedDescription(void) { return "Established"; }
ConnectionState establishedOpen(void)       { return makeEstablishedState(); }
ConnectionState establishedReceiveSyn(void) { return makeEstablishedState(); }
ConnectionState establishedEstablish(void)  { return makeEstablishedState(); }
ConnectionState establishedClose(void)      { return makeClosedState(); }

ConnectionState makeEstablishedState(void) {
    ConnectionState s;
    s.id = STATE_ESTABLISHED;
    s.description = establishedDescription;
    s.open = establishedOpen;
    s.receiveSyn = establishedReceiveSyn;
    s.establish = establishedEstablish;
    s.close = establishedClose;
    return s;
}

int main(void) {
    ConnectionState conn = makeClosedState();
    printf("%s\n", conn.description());   // Closed

    conn = conn.open();
    printf("%s\n", conn.description());   // Listen

    conn = conn.receiveSyn();
    printf("%s\n", conn.description());   // SynReceived

    conn = conn.establish();
    printf("%s\n", conn.description());   // Established

    conn = conn.receiveSyn();              // invalid here
    printf("%s\n", conn.description());   // Established (unchanged)

    conn = conn.close();
    printf("%s\n", conn.description());   // Closed

    return 0;
}

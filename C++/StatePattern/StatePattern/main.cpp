//
//  main.cpp
//  StatePattern
//
//  Created by Anussha on 07/09/26.
//

#include <iostream>
#include <memory>

class ConnectionState {
public:
    virtual ~ConnectionState() = default;
    virtual std::unique_ptr<ConnectionState> open() = 0;
    virtual std::unique_ptr<ConnectionState> receiveSyn() = 0;
    virtual std::unique_ptr<ConnectionState> establish() = 0;
    virtual std::unique_ptr<ConnectionState> close() = 0;
    virtual std::string description() const = 0;
};

class ListenState;
class SynReceivedState;
class EstablishedState;

class ClosedState : public ConnectionState {
public:
    std::unique_ptr<ConnectionState> open() override;
    std::unique_ptr<ConnectionState> receiveSyn() override;
    std::unique_ptr<ConnectionState> establish() override;
    std::unique_ptr<ConnectionState> close() override;
    std::string description() const override { return "Closed"; }
};

class ListenState : public ConnectionState {
public:
    std::unique_ptr<ConnectionState> open() override;
    std::unique_ptr<ConnectionState> receiveSyn() override;
    std::unique_ptr<ConnectionState> establish() override;
    std::unique_ptr<ConnectionState> close() override;
    std::string description() const override { return "Listen"; }
};

class SynReceivedState : public ConnectionState {
public:
    std::unique_ptr<ConnectionState> open() override;
    std::unique_ptr<ConnectionState> receiveSyn() override;
    std::unique_ptr<ConnectionState> establish() override;
    std::unique_ptr<ConnectionState> close() override;
    std::string description() const override { return "SynReceived"; }
};

class EstablishedState : public ConnectionState {
public:
    std::unique_ptr<ConnectionState> open() override;
    std::unique_ptr<ConnectionState> receiveSyn() override;
    std::unique_ptr<ConnectionState> establish() override;
    std::unique_ptr<ConnectionState> close() override;
    std::string description() const override { return "Established"; }
};

// ClosedState
std::unique_ptr<ConnectionState> ClosedState::open() {
    return std::make_unique<ListenState>();
}
std::unique_ptr<ConnectionState> ClosedState::receiveSyn() {
    return std::make_unique<ClosedState>();   // invalid here, stay put
}
std::unique_ptr<ConnectionState> ClosedState::establish() {
    return std::make_unique<ClosedState>();
}
std::unique_ptr<ConnectionState> ClosedState::close() {
    return std::make_unique<ClosedState>();
}

// ListenState
std::unique_ptr<ConnectionState> ListenState::open() {
    return std::make_unique<ListenState>();
}
std::unique_ptr<ConnectionState> ListenState::receiveSyn() {
    return std::make_unique<SynReceivedState>();
}
std::unique_ptr<ConnectionState> ListenState::establish() {
    return std::make_unique<ListenState>();
}
std::unique_ptr<ConnectionState> ListenState::close() {
    return std::make_unique<ClosedState>();
}

// SynReceivedState
std::unique_ptr<ConnectionState> SynReceivedState::open() {
    return std::make_unique<SynReceivedState>();
}
std::unique_ptr<ConnectionState> SynReceivedState::receiveSyn() {
    return std::make_unique<SynReceivedState>();
}
std::unique_ptr<ConnectionState> SynReceivedState::establish() {
    return std::make_unique<EstablishedState>();
}
std::unique_ptr<ConnectionState> SynReceivedState::close() {
    return std::make_unique<ClosedState>();
}

// EstablishedState
std::unique_ptr<ConnectionState> EstablishedState::open() {
    return std::make_unique<EstablishedState>();
}
std::unique_ptr<ConnectionState> EstablishedState::receiveSyn() {
    return std::make_unique<EstablishedState>();
}
std::unique_ptr<ConnectionState> EstablishedState::establish() {
    return std::make_unique<EstablishedState>();
}
std::unique_ptr<ConnectionState> EstablishedState::close() {
    return std::make_unique<ClosedState>();
}

class Connection {
public:
    Connection() : state(std::make_unique<ClosedState>()) {}

    void open()       { state = state->open(); }
    void receiveSyn() { state = state->receiveSyn(); }
    void establish()  { state = state->establish(); }
    void close()      { state = state->close(); }

    std::string description() const { return state->description(); }

private:
    std::unique_ptr<ConnectionState> state;
};

int main() {
    Connection conn;
    std::cout << conn.description() << "\n";   // Closed

    conn.open();
    std::cout << conn.description() << "\n";   // Listen

    conn.receiveSyn();
    std::cout << conn.description() << "\n";   // SynReceived

    conn.establish();
    std::cout << conn.description() << "\n";   // Established

    conn.receiveSyn();                          // invalid here
    std::cout << conn.description() << "\n";   // Established (unchanged)

    conn.close();
    std::cout << conn.description() << "\n";   // Closed

    return 0;
}

//
//  main.cpp
//  ObserverPattern
//
//  Created by Anussha on 03/09/26.
//

#include <iostream>
#include <string>
#include <vector>

class Observer {
public:
    virtual void update(int newValue) = 0 ;
    virtual ~Observer() = default;
};

class ConsoleLogger : public Observer {
public:
    void update(int newValue) override {
        std::cout << "ConsoleLogger: value changed to " << newValue << std::endl;
    }
};

class AlertObserver : public Observer {
public:
    void update(int newValue) override {
        if (newValue > 100) {
            std::cout << "AlertObserver: ALERT! Value " << newValue << " exceeds threshold!" << std::endl;
        } else {
            std::cout << "AlertObserver: value " << newValue << " is within normal range." << std::endl;
        }
    }
};

class Subject {
private:std::vector<Observer*> observers;
public:
    void attach(Observer* observer){
        observers.push_back(observer);
    }
    void notifyAll(int newValue){
        for(Observer* observer : observers){
            observer->update(newValue);
        }
    }
};

// Usage:
int main() {
    ConsoleLogger logger;
    AlertObserver alert;

    Subject subject;
    subject.attach(&logger);
    subject.attach(&alert);

    subject.notifyAll(150);
    // ConsoleLogger: value changed to 150
    // AlertObserver: ALERT! Value 150 exceeds threshold!

    subject.notifyAll(50);
    // ConsoleLogger: value changed to 50
    // AlertObserver: value 50 is within normal range.

    return 0;
}



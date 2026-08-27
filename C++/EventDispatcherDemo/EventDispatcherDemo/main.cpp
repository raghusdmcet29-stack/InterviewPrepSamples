//
//  main.cpp
//  EventDispatcherDemo
//
//  Created by Anussha on 27/08/26.
//

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::function<void(int)> callback = [](int x){
    std::cout << "Got: " << x << std::endl;
};

class EventDispatcher {
public:
    using Callback = std::function<void(int)>;
    
    void subscribe(const std::string& eventName,Callback cb){
        listeners[eventName].push_back(cb);
    }
    
    void fire(const std::string& eventName,int payload){
        auto it = listeners.find(eventName);
        if(it == listeners.end()) return;
        for (auto& cb: it->second) {
            cb(payload);
        }
    }
private:
    std::unordered_map<std::string, std::vector<Callback>> listeners;
};


int main(){
    callback(42); // prints "Got: 42"
    EventDispatcher dispatcher;
    
    dispatcher.subscribe("damage", [](int amount) {
            std::cout << "Logger: took " << amount << " damage" << std::endl;
        });
    
    dispatcher.subscribe("damage", [](int amount) {
            std::cout << "UI: update health bar by -" << amount << std::endl;
        });

        dispatcher.subscribe("heal", [](int amount) {
            std::cout << "Logger: healed " << amount << " HP" << std::endl;
        });
    
    std::cout << "--- Firing 'damage' with 25 ---" << std::endl;
       dispatcher.fire("damage", 25);

       std::cout << "--- Firing 'heal' with 10 ---" << std::endl;
       dispatcher.fire("heal", 10);

       std::cout << "--- Firing 'unknown_event' (no listeners) ---" << std::endl;
       dispatcher.fire("unknown_event", 999);
       std::cout << "(nothing should print above this line for unknown_event)" << std::endl;

    return 0;
}

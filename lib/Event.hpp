#pragma once
#include <functional>
#include <vector>

template<typename... Args>
class Event {
public:
    using Listener = std::function<void(Args...)>;

    void subscribe(Listener listener) {
        listeners.push_back(listener);
    }

    void notify(Args... args) {
        for (auto& l : listeners) {
            l(args...);
        }
    }

    void subscribeOnce(Listener listener) {
        bool* executed = new bool(false); // flag dynamique pour être capturé par lambda
        listeners.push_back([listener, executed](Args... args){
            if (!*executed) {
                listener(args...);
                *executed = true;
            }
        });
    }

private:
    std::vector<Listener> listeners;
};



#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <string>
#include "WorldEntity.hpp"

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
        auto executed = std::make_shared<bool>(false); 
        listeners.push_back([listener, executed](Args... args){
            if (!*executed) {
                *executed = true;
                listener(args...);
            }
        });
    }

private:
    std::vector<Listener> listeners;
};

class WorldEntity; // Forward declaration

struct GameEvents {
    static Event<int, int> OnPlayerMove; 
    static Event<std::string> OnDialogueTrigger;
    static Event<WorldEntity*> OnEntityDestroyed;
    static Event SaveGame;
    static Event YesChoice;
    static Event NoChoice;
    static Event<> Ev1;
    static Event<> Ev2;
    static Event<> Ev3;
};


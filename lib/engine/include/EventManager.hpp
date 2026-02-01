#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <unordered_map>
#include <string>
#include "Event.hpp"

class EventManager {
public:
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }

    // Accès aux events
    void launchEvent(const std::string& name) {
        auto it = justEvents.find(name);
        if (it != justEvents.end()) {
            it->second->notify();
        }
    }

    void makeChoice(std::string dialoguekey);
    std::unordered_map<std::string, Event<>*> justEvents {
        {"SaveGame", &GameEvents::SaveGame},
        {"YesChoice", &GameEvents::YesChoice},
        {"NoChoice", &GameEvents::NoChoice},
        {"Ev1", &GameEvents::Ev1},
        {"Ev2", &GameEvents::Ev2},
        {"Ev3", &GameEvents::Ev3}
    };


private:
    EventManager();


    // --- INTERDIRE LA COPIE ---
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
};

#endif

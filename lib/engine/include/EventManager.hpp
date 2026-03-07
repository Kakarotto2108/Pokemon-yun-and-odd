#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <unordered_map>
#include <string>
#include "Event.hpp"
#include "GameChoiceBox.hpp"

class EventManager {
public:
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }

    // Accès aux events
    void launchEvent(const std::string& name, const std::string& arg = "") {
        if (arg.empty()) {
            auto it = justEvents.find(name);
            if (it != justEvents.end()) {
                it->second->notify();
            }
        } else {
            auto it = stringEvents.find(name);
            if (it != stringEvents.end()) {
                it->second->notify(arg);
            }
        }
    }

    void makeChoice(std::string dialoguekey);
    std::unordered_map<std::string, Event<>*> justEvents {
        {"SaveGame", &GameEvents::SaveGame},
        {"YesChoice", &GameEvents::YesChoice},
        {"NoChoice", &GameEvents::NoChoice},
        {"Ev1", &GameEvents::Ev1},
        {"Ev2", &GameEvents::Ev2},
        {"Ev3", &GameEvents::Ev3},
        {"OpenBag", &GameEvents::OpenBag},
        {"CloseBag", &GameEvents::CloseBag},
        {"OpenPokedex", &GameEvents::OpenPokedex},
        {"OpenPokemon", &GameEvents::OpenPokemon},
        {"OpenPlayer", &GameEvents::OpenPlayer},
        {"OpenOptions", &GameEvents::OpenOptions},
        {"ViewPokemon", &GameEvents::ViewPokemon},
        {"SummaryChoice", &GameEvents::SummaryChoice},
        {"ItemsChoice", &GameEvents::ItemsChoice},
        {"SelectGiveItem", &GameEvents::SelectGiveItem},
        {"TakeItem", &GameEvents::TakeItem},
        {"SwitchItem", &GameEvents::SwitchItem}
    };

    std::unordered_map<std::string, Event<std::string>*> stringEvents {
        {"MoveOrder", &GameEvents::MoveOrder},
        {"OrderChoice", &GameEvents::OrderChoice},
        {"GiveItem", &GameEvents::GiveItem},
    };


private:
    EventManager();

    GameChoiceBox m_choiceBox;
    // --- INTERDIRE LA COPIE ---
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
};

#endif

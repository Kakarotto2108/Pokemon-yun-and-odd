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
    void launchEvent(const std::string& name, const ChoiceParam& param) {
        std::visit([&](auto&& value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                auto it = justEvents.find(name);
                if (it != justEvents.end()) {
                    it->second->notify();
                }
            } else if constexpr (std::is_same_v<T, std::string>){
                auto it = stringEvents.find(name);
                if (it != stringEvents.end()) {
                    it->second->notify(value);
                }
            } else if constexpr (std::is_same_v<T, int>) {
                auto it = intEvents.find(name);
                if (it != intEvents.end()) {
                    it->second->notify(value);
                }
            }
        }, param);
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
        {"SwitchItem", &GameEvents::SwitchItem},
        {"GiveItem", &GameEvents::GiveItem},
        {"YesGiveItem", &GameEvents::YesGiveItem},
        {"Cancel", &GameEvents::Cancel},
        {"ViewItem", &GameEvents::ViewItem},
        {"UseItem", &GameEvents::UseItem},
        {"DiscardItem", &GameEvents::DiscardItem},
        {"SelectDiscardItem", &GameEvents::SelectDiscardItem},
    };

    std::unordered_map<std::string, Event<std::string>*> stringEvents {
        {"MoveOrder", &GameEvents::MoveOrder},
        {"OrderChoice", &GameEvents::OrderChoice}
    };

    std::unordered_map<std::string, Event<int>*> intEvents {
    };

private:
    EventManager();

    GameChoiceBox m_choiceBox;

    // --- INTERDIRE LA COPIE ---
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
};

#endif

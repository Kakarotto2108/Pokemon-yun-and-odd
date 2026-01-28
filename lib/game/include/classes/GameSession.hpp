#pragma once
#include <set>
#include <string>
#include "ZoneFactory.hpp"

class GameSession {
public:
    static GameSession& getInstance() {
        static GameSession instance;
        return instance;
    }

    void addCollectedItem(const std::string& id) { m_collectedItems.insert(id); }
    bool hasCollectedItem(const std::string& id) const { return m_collectedItems.count(id); }

private:
    GameSession() = default;
    std::set<std::string> m_collectedItems;
};
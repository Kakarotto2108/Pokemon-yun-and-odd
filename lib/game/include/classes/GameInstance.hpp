#pragma once
#include <unordered_map>
#include "ZoneState.hpp"

class GameInstance {
public:
    static GameInstance& getInstance() {
        static GameInstance instance;
        return instance;
    }

    ZoneState& getZoneState(int zoneId) {
        return m_zones[zoneId];
    }

    void saveZoneState(int zoneId, const std::vector<std::unique_ptr<WorldEntity>>& entities) {
        ZoneState& state = m_zones[zoneId];
        state.entities.clear();
        for (const auto& entity : entities) {
            state.entities[entity->getName()] = entity->getState();
        }
    }

private:
    GameInstance() = default;
    std::unordered_map<int, ZoneState> m_zones;
};

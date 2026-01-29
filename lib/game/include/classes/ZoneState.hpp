#pragma once
#include <unordered_map>
#include <string>
#include "EntityState.hpp"
#include <nlohmann/json.hpp>

struct ZoneState {
    std::unordered_map<std::string, EntityState> entities;

    nlohmann::json toJson() const {
        nlohmann::json j;
        for (const auto& [name, state] : entities) {
            j[name] = state.toJson();  // <- fonctionne maintenant
        }
        return j;
    }

    static ZoneState fromJson(const nlohmann::json& j) {
        ZoneState zs;
        for (auto it = j.begin(); it != j.end(); ++it) {
            zs.entities[it.key()] = EntityState::fromJson(it.value()); // <- aussi ok
        }
        return zs;
    }
};

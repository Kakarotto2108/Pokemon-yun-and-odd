#pragma once
#include <SFML/System.hpp>
#include <nlohmann/json.hpp>
#include <string>

enum class EntityType {
    NPC,
    OBJ,
    IOG
};

struct EntityState {
    std::string texturePath;
    EntityType type;
    sf::Vector2i position;
    int orientation = 0;
    std::string dialogKey;

    // ---------------- Serialize ----------------
    nlohmann::json toJson() const {
        nlohmann::json j;
        j["x"] = position.x;
        j["y"] = position.y;
        j["orientation"] = orientation;
        j["dialogKey"] = dialogKey;
        j["texturePath"] = texturePath;
        j["type"] = static_cast<int>(type);
        return j;
    }

    // ---------------- Deserialize ----------------
    static EntityState fromJson(const nlohmann::json& j) {
        EntityState state;
        state.position.x = j.value("x", 0);
        state.position.y = j.value("y", 0);
        state.orientation = j.value("orientation", 0);
        state.dialogKey = j.value("dialogKey", "");
        state.texturePath = j.value("texturePath", "");
        state.type = static_cast<EntityType>(j.value("type", 0));

        if(state.position.x < 0) state.position.x = 0;
        if(state.position.y < 0) state.position.y = 0;
        if(state.orientation < 0 || state.orientation > 3) state.orientation = 0;

        return state;
    }
};

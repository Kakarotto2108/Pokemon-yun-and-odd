#pragma once
#include <SFML/System.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include "Inventory.hpp"

enum class EntityType {
    PLAYER,
    NPC,
    OBJ,
    IOG
};

struct EntityState {
    std::string texturePath;
    EntityType type;
    sf::Vector2i position;
    std::optional<sf::Vector2f> size;
    int orientation = 0;
    std::string dialogKey;
    std::unique_ptr<Inventory> inventory = nullptr;

    // ---------------- Serialize ----------------
    nlohmann::json toJson() const {
        nlohmann::json j;
        j["x"] = position.x;
        j["y"] = position.y;
        j["orientation"] = orientation;
        j["dialogKey"] = dialogKey;
        j["texturePath"] = texturePath;
        j["type"] = static_cast<int>(type);
        if (inventory)
            j["inventory"] = inventory->toJson();
        else
            j["inventory"] = nlohmann::json::array();
        if (size) {
            std::cout << "sizeX et sizeY : " << size->x << " " << size->y << std::endl;
            j["sizeX"] = size->x;
            j["sizeY"] = size->y;
}
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
        if (!j["inventory"].is_null())
            state.inventory = std::make_unique<Inventory>(Inventory::fromJson(j["inventory"]));
        else
            state.inventory = std::make_unique<Inventory>();
        if (j.contains("sizeX") && j.contains("sizeY")) {
            state.size = sf::Vector2f(
                j["sizeX"].get<float>(),
                j["sizeY"].get<float>()
            );
        }


        if(state.position.x < 0) state.position.x = 0;
        if(state.position.y < 0) state.position.y = 0;
        if(state.orientation < 0 || state.orientation > 3) state.orientation = 0;

        return state;
    }

    void printDebug() {
        std::cout << "EntityState Debug:" << std::endl;
        std::cout << "  Texture Path: " << texturePath << std::endl;
        std::cout << "  Type: " << static_cast<int>(type) << std::endl;
        std::cout << "  Position: (" << position.x << ", " << position.y << ")" << std::endl;
        std::cout << "  Orientation: " << orientation << std::endl;
        std::cout << "  Dialog Key: " << dialogKey << std::endl;
        if (inventory) {
            std::cout << "  Inventory: " << std::endl;
            inventory->debugPrint();
        } else {
            std::cout << "  Inventory: None" << std::endl;
        }
    }
};

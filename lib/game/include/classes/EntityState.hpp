#pragma once
#include <SFML/System.hpp>
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
};

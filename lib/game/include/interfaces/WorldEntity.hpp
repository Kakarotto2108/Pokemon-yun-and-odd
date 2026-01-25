#pragma once
#include <SFML/Graphics.hpp>

class WorldEntity {
public:
    virtual ~WorldEntity() = default;

    virtual sf::Vector2i getPosition() const = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};

#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class WorldEntity {
public:
    WorldEntity(const std::string& name, sf::Vector2i pos) 
        : m_name(name), m_logicalPos(pos) {}

    virtual ~WorldEntity() = default;

    // Méthodes communes à toutes les entités
    virtual void draw(sf::RenderWindow& window) const = 0;
    sf::Vector2i getPosition() const { return m_logicalPos; }
    std::string getName() const { return m_name; }

protected:
    std::string m_name;
    sf::Vector2i m_logicalPos;
};
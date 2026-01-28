#pragma once
#include "Interactable.hpp"
#include "WorldEntity.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

class Iog : public WorldEntity, public Interactable {
public:
    Iog(std::string name,sf::Vector2i pos);
    
    void interact() override;
    const sf::Sprite& getSprite() const { return m_sprite; }
    void draw(sf::RenderWindow& window) const override;
    ~Iog() override = default;    

private:
    sf::Sprite m_sprite;
    std::string m_name;
};
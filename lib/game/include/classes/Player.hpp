#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Zone.hpp"
#include "Inventory.hpp"
#include "WorldEntity.hpp"
#include "Event.hpp"

class Player : public WorldEntity {
public:
    Player(const std::string& name);

    void moveRequest(sf::Vector2i direction, Zone& zone);
    void update();
    void draw(sf::RenderWindow& window) const;

    // Getters
    sf::Vector2i getPosition() const;
    sf::Vector2f getDrawPosition() const;
    sf::Vector2i getFacingTile() const;
    
    // Setters
    void setLogicalPos(const sf::Vector2i& pos);
    void receiveItem(const Item& item);

    std::string m_name;

private:
    void updateTexture();

    // Ordre respecté pour éviter Wreorder
    sf::Vector2i logicalPos;
    int orientation; // 0:Bas, 1:Gauche, 2:Droite, 3:Haut
    sf::Vector2f targetPos;
    
    sf::Texture texDown, texLeft, texRight, texUp;
    mutable sf::Sprite sprite;
    Inventory m_inventory;

    const float TILE_SIZE = 32.f;
    const float MOVE_SPEED = 0.2f; 
};
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Item.hpp"
#include "Inventory.hpp"
#include "Zone.hpp"
#include "EntiteMonde.hpp"
#include "Interactable.hpp"

class Player : public EntiteMonde {
public:
    Player(const std::string& downTexPath,
           const std::string& leftTexPath,
           const std::string& rightTexPath,
           const std::string& upTexPath,
           const std::string& name
            );

    void changeZone(std::vector<std::string> collisionMap, Zone& zone);
    void handleInput(sf::RenderWindow& window, Zone& zone, float delay);
    void update();
    
    std::string m_name;
    sf::Vector2i getPosition() const override;       // position logique (tile)
    void draw(sf::RenderWindow& window) const override;
    sf::Vector2i getFacingTile() const;     // tile devant le joueur
    sf::Vector2f getDrawPosition() const;   // position en pixels (sprite)
    sf::Vector2i logicalPos;   // position sur la grille
    int getCurrentZone() const;
    void setLogicalPos(const sf::Vector2i& pos);

private:
    sf::Texture texDown, texLeft, texRight, texUp;
    sf::Sprite sprite;
    Inventory m_inventory;

    int orientation;           // 0: down, 1: left, 2: right, 3: up

    sf::Vector2f targetPos;    // position pixel cible pour déplacement fluide
};

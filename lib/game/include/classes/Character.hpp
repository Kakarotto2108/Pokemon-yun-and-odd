#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>

#include "WorldEntity.hpp"
#include "Animation.hpp"
#include "Zone.hpp"
#include "Item.hpp"
#include "Inventory.hpp"

class Character : public WorldEntity {
public:
    Character(const std::string& name, const std::string& spriteSheetName, const sf::Vector2i& pos, int orientation);
    virtual ~Character() = default;

    virtual void moveRequest(sf::Vector2i direction, Zone& zone);
    virtual void update(float dt);
    void draw(sf::RenderWindow& window) const;
    void stopAnimation();

    sf::Vector2i getPosition() const;
    const sf::Sprite& getSprite() const { return m_sprite; }
    sf::Vector2f getDrawPosition() const;
    sf::Vector2i getFacingTile() const;
    void setLogicalPos(const sf::Vector2i& pos);
    std::string getName() const { return m_name; }
    bool getIsMoving() const { return m_isMoving; }

    void receiveItem(const Item& item);
    Inventory& getInventory() { return m_inventory; }

protected:
    std::string m_name;
    sf::Vector2f m_targetPos;
    int m_orientation;
    
    sf::Sprite m_sprite;
    std::map<std::string, Animation> m_animations;
    std::string m_currentAnim;
    bool m_isMoving;

    const float TILE_SIZE = 32.f;
    const float MOVE_SPEED = 0.2f; 

    Inventory m_inventory;
};

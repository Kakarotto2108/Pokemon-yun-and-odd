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
#include "CharacterPath.hpp"
#include "EntityState.hpp"

class Zone;
class CharacterPath;

class Character : public WorldEntity {
public:
    Character(const std::string& name, const std::string& spriteSheetName, const sf::Vector2i& pos, int orientation, std::unique_ptr<CharacterPath> path = nullptr, const Inventory& inventory = Inventory());
    virtual ~Character() = default;

    virtual void moveRequest(sf::Vector2i direction, Zone& zone);
    virtual void update(float dt, Zone& zone);
    void draw(sf::RenderWindow& window) const;
    void stopAnimation();

    sf::Vector2i getPosition() const;
    const sf::Sprite& getSprite() const { return m_sprite; }
    sf::Vector2f getDrawPosition() const;
    sf::Vector2i getFacingTile() const;
    int getOrientation() const { return m_orientation; }
    void setOrientation(int orientation);
    void setLogicalPos(const sf::Vector2i& pos);
    std::string getName() const { return m_name; }
    bool getIsMoving() const { return m_isMoving; }

    Inventory& getInventory() { return *m_inventory; }
    void setInventory(const Inventory& inventory) {
        m_inventory = std::make_unique<Inventory>(inventory);
    }
    void receiveItem(const Item& item);
    void RemoveItem(const Item& item, int quantity = 1);

    void setPath(std::unique_ptr<CharacterPath> path) { m_path = std::move(path); }
    bool hasPath() const { return m_path != nullptr; }
    CharacterPath* getPath() const { return m_path.get(); }

    void setMoveDelay(float delay) { m_moveDelay = delay; }
    float getMoveDelay() const { return m_moveDelay; }

    void setCollision(bool collision) { isColliding = collision; }
    bool getCollision() const { return isColliding; }

    void applyState(const EntityState& state) override {
        WorldEntity::applyState(state);
        setOrientation(state.orientation);
        if (state.inventory)
            m_inventory = std::make_unique<Inventory>(*state.inventory);
    }

    EntityState getState() const override {
        EntityState state = WorldEntity::getState();
        state.texturePath = m_texturePath;
        state.orientation = m_orientation;
        state.inventory = std::make_unique<Inventory>(*m_inventory);
        return state;
    }

    
protected:
    std::string m_name;
    std::string m_texturePath;
    sf::Vector2f m_targetPos;
    int m_orientation;
    
    sf::Sprite m_sprite;
    std::map<std::string, Animation> m_animations;
    std::string m_currentAnim;
    bool m_isMoving;

    const float TILE_SIZE = 32.f;
    const float MOVE_SPEED = 0.2f; 

    std::unique_ptr<Inventory> m_inventory;
    std::unique_ptr<CharacterPath> m_path;
    float m_moveDelay = 0.2f; 
    bool isColliding = true;
};

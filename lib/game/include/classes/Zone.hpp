#ifndef ZONE_HPP
#define ZONE_HPP

#include <vector>
#include <memory>
#include "WorldEntity.hpp"
#include "Interactable.hpp"
#include "TileMap.hpp"

struct ZoneTransition {
    int targetZoneId;
    sf::Vector2i targetSpawnPos;
};

class Zone {
public:
    Zone(int id, unsigned int width, unsigned int height,
         std::vector<int> collisionMap,
         std::vector<std::unique_ptr<Interactable>> entities,
         sf::Texture& tileset,
         const std::vector<int>& visualMap);

    int getId() const { return m_id; }
    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }
    const std::vector<int>& getCollisionMap() const { return m_collisionMap; }
    std::vector<ZoneTransition>& getTransitions() { return m_transitions; }

    void updateInteractableGrid();
    bool isBlocking(int x, int y) const;
    Interactable* getInteractableAt(int x, int y) const;
    void drawAll(sf::RenderWindow& window, const WorldEntity& player);

private:
    int m_id;
    unsigned int m_width, m_height;
    std::vector<int> m_collisionMap;
    std::vector<ZoneTransition> m_transitions;
    std::vector<std::unique_ptr<Interactable>> m_entities; // 1. Entities
    sf::Texture* m_tileset;                               // 2. Tileset
    
    std::vector<std::vector<Interactable*>> m_interactables;
    TileMap m_tileMap;
};

#endif
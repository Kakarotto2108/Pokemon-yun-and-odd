#ifndef ZONE_HPP
#define ZONE_HPP

#include <vector>
#include <memory>
#include "WorldEntity.hpp"
#include "TileMap.hpp"

struct ZoneTransition {
    int targetZoneId;
    sf::Vector2i targetSpawnPos;
};

class Zone {
public:
    Zone(int id, unsigned int width, unsigned int height, std::vector<int> collisionMap, std::vector<std::unique_ptr<WorldEntity>> entities, sf::Texture& tileset, const std::vector<int>& visualMap);

    int getId() const { return m_id; }
    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }
    const std::vector<int>& getCollisionMap() const { return m_collisionMap; }
    std::vector<ZoneTransition>& getTransitions() { return m_transitions; }

    bool isBlocking(int x, int y) const;
    void drawAll(sf::RenderWindow& window, const WorldEntity& player);
    WorldEntity* getEntityAt(int x, int y) const {
        for (const auto& ent : m_entities) {
            sf::Vector2i pos = ent->getPosition();
            if (pos.x == x && pos.y == y) {
                return ent.get();
            }
        }
        return nullptr;
    }

private:
    int m_id;
    unsigned int m_width, m_height;
    std::vector<int> m_collisionMap;
    std::vector<ZoneTransition> m_transitions;
    std::vector<std::unique_ptr<WorldEntity>> m_entities;
    sf::Texture* m_tileset;
    
    TileMap m_tileMap;
};

#endif
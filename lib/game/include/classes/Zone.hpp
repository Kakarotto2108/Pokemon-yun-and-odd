#ifndef ZONE_HPP
#define ZONE_HPP

#include <vector>
#include <memory>
#include "WorldEntity.hpp"
#include "TileMap.hpp"
#include "Obj.hpp"
#include <SFML/Graphics.hpp>

struct ZoneTransition {
    int targetZoneId;
    sf::Vector2i targetSpawnPos;
};

class Zone {
public:
    Zone(int id, unsigned int width, unsigned int height, unsigned int widthColl, unsigned int heightColl, sf::Vector2i spawnPos, std::map<int,sf::Vector2i> spawnPoints, std::vector<int> collisionMap, std::vector<std::unique_ptr<WorldEntity>> entities, sf::Texture& tileset, const std::vector<int>& visualMap);

    int getId() const { return m_id; }
    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }
    unsigned int getCollisionWidth() const { return m_widthColl; }
    const std::vector<int>& getCollisionMap() const { return m_collisionMap; }
    const TileMap& getTileMap() const { return m_tileMap; }
    const sf::Texture& getTileset() const { return *m_tileset; }
    const std::vector<std::unique_ptr<WorldEntity>>& getEntities() const { return m_entities; }
    void destroyCollision(int x, int y);
    
    bool isBlocking(int x, int y) const;
    bool removeEntity(WorldEntity* entity);
    WorldEntity* getEntityAt(int x, int y) const {
        for (const auto& ent : m_entities) {
            Obj* obj = dynamic_cast<Obj*>(ent.get());
            if (obj) {
                for (const auto& pos : obj->getAllPositions()){
                    if (pos.first == x && pos.second == y) {
                        return ent.get();
                    }
                }
            }
            else {
                sf::Vector2i pos = ent->getPosition();
                if (pos.x == x && pos.y == y) {
                    return ent.get();
                }
            }
        }
        return nullptr;
    }

    sf::Vector2i getSpawnPos(int index) const {
        auto it = m_spawnPoints.find(index);
        if (it != m_spawnPoints.end()) {
            return it->second;
        }
        return m_spawnPos;
    }

private:
    int m_id;
    unsigned int m_width, m_height;
    unsigned int m_widthColl, m_heightColl;
    sf::Vector2i m_spawnPos;
    std::map<int, sf::Vector2i> m_spawnPoints;
    std::vector<int> m_collisionMap;
    std::vector<std::unique_ptr<WorldEntity>> m_entities;
    sf::Texture* m_tileset;
    
    TileMap m_tileMap;
};

#endif
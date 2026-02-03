#include "Zone.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include <algorithm>
#include <iostream>

Zone::Zone(int id, unsigned int width, unsigned int height,
           sf::Vector2i spawnPos,
           std::map<int, sf::Vector2i> spawnPoints,
           std::vector<int> collisionMap,
           std::vector<std::unique_ptr<WorldEntity>> entities,
           sf::Texture& tileset,
           const std::vector<int>& visualMap)
    : m_id(id), m_width(width), m_height(height)
    , m_spawnPos(spawnPos)
    , m_spawnPoints(std::move(spawnPoints))
    , m_collisionMap(std::move(collisionMap))
    , m_entities(std::move(entities))
    , m_tileset(&tileset)
{
    m_tileMap.load(*m_tileset, sf::Vector2u(32, 32), visualMap, m_width, m_height);
}

bool Zone::isBlocking(int x, int y) const {
    if (x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height) return true;
    for (const auto& ent : m_entities) {
        Character* character = dynamic_cast<Character*>(ent.get());
        if (character && character->getCollision()) {
            sf::Vector2i pos = character->getPosition();
            if (pos.x == x && pos.y == y) {
                return true;
            }
        }
        Obj* obj = dynamic_cast<Obj*>(ent.get());
        if (obj && obj->getCollision()) {
            for (const auto& pos : obj->getAllPositions()){
                if (pos.first == x && pos.second == y) {
                    return true;
                }
            }
        }
    }
    if (Player::getInstance().getCollision()) {
        sf::Vector2i playerPos = Player::getInstance().getPosition();
        if (playerPos.x == x && playerPos.y == y) {
            return true;
        }
    }
    return m_collisionMap[x + y * m_width] == -1;
}

void Zone::destroyCollision(int x, int y) {
    if (x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height) return;
    m_collisionMap[x + y * m_width] = 0;
}

bool Zone::removeEntity(WorldEntity* entity) {
    auto it = std::remove_if(m_entities.begin(), m_entities.end(), 
        [entity](const std::unique_ptr<WorldEntity>& ptr) {
            return ptr.get() == entity;
        });
    
    if (it != m_entities.end()) {
        m_entities.erase(it, m_entities.end());
        return true;
    }
    return false;
}
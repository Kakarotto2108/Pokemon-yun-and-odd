#include "Zone.hpp"
#include <algorithm>

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

void Zone::drawAll(sf::RenderWindow& window, const WorldEntity& player) {
    //window.draw(m_tileMap);

    std::vector<const WorldEntity*> renderQueue;
    renderQueue.push_back(&player);
    
    for (const auto& e : m_entities) {
        renderQueue.push_back(dynamic_cast<const WorldEntity*>(e.get()));
    }

    std::sort(renderQueue.begin(), renderQueue.end(), [](const WorldEntity* a, const WorldEntity* b) {
        return a->getPosition().y < b->getPosition().y;
    });

    for (const auto* e : renderQueue) e->draw(window);
}

bool Zone::isBlocking(int x, int y) const {
    if (x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height) return true;
    return m_collisionMap[x + y * m_width] == -1;
}
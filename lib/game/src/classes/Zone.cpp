#include "Zone.hpp"
#include <algorithm>

Zone::Zone(int id, unsigned int width, unsigned int height,
           std::vector<int> collisionMap,
           std::vector<std::unique_ptr<Interactable>> entities,
           sf::Texture& tileset,
           const std::vector<int>& visualMap)
    : m_id(id), m_width(width), m_height(height)
    , m_collisionMap(std::move(collisionMap))
    , m_entities(std::move(entities))
    , m_tileset(&tileset)
{
    m_tileMap.load(*m_tileset, sf::Vector2u(32, 32), visualMap, m_width, m_height);
    
    m_interactables.assign(m_height, std::vector<Interactable*>(m_width, nullptr));
    updateInteractableGrid();
}

void Zone::drawAll(sf::RenderWindow& window, const WorldEntity& player) {
    window.draw(m_tileMap);

    std::vector<const WorldEntity*> renderQueue;
    renderQueue.push_back(&player);
    
    for (const auto& e : m_entities) {
        renderQueue.push_back(static_cast<const WorldEntity*>(e.get()));
    }

    std::sort(renderQueue.begin(), renderQueue.end(), [](const WorldEntity* a, const WorldEntity* b) {
        return a->getPosition().y < b->getPosition().y;
    });

    for (const auto* e : renderQueue) e->draw(window);
}

void Zone::updateInteractableGrid() {
    for (auto& row : m_interactables) std::fill(row.begin(), row.end(), nullptr);
    for (auto& ent : m_entities) {
        sf::Vector2i pos = ent->getPosition();
        if (pos.x >= 0 && pos.x < (int)m_width && pos.y >= 0 && pos.y < (int)m_height)
            m_interactables[pos.y][pos.x] = ent.get();
    }
}

bool Zone::isBlocking(int x, int y) const {
    if (x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height) return true;
    return m_collisionMap[x + y * m_width] != 0;
}

Interactable* Zone::getInteractableAt(int x, int y) const {
    if (x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height) return nullptr;
    return m_interactables[y][x];
}
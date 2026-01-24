#include "Zone.hpp"
#include "MapLoader.hpp"
#include "EntiteMonde.hpp"
#include <iostream>

const int TILE_SIZE = 32;

Zone::Zone(int id, 
           std::vector<std::unique_ptr<Pnj>> pnjs, 
           std::vector<std::unique_ptr<Obj>> objs)
    : m_id(id)
    , m_pnjs(std::move(pnjs))
    , m_objs(std::move(objs)) 
{
    loadData();
    updateInteractableGrid();
}

void Zone::loadData() {
    std::string baseDir = "assets/zone/zone" + std::to_string(m_id) + "/";

    // 1. Chargement des fichiers via MapLoader
    m_visualMap = MapLoader::loadFromFile(baseDir + "map.txt", m_width, m_height);
    m_collisionMap = MapLoader::loadFromFile(baseDir + "collisionMap.txt", m_width, m_height);

    // 2. Initialisation du rendu graphique
    if (!m_tileMap.load(baseDir + "tileset.png", sf::Vector2u(TILE_SIZE, TILE_SIZE), m_visualMap, m_width, m_height)) {
        std::cerr << "Erreur : Impossible de charger le visuel de la zone " << m_id << std::endl;
    }
}

void Zone::updateInteractableGrid() {
    m_interactables.assign(m_height, std::vector<Interactable*>(m_width, nullptr));

    for (auto& pnj : m_pnjs) {
        // Remplacement de getGridPosition par getPosition
        sf::Vector2i pos = sf::Vector2i(pnj->getPosition().x, pnj->getPosition().y); 
        if (pos.y >= 0 && pos.y < (int)m_height && pos.x >= 0 && pos.x < (int)m_width)
            m_interactables[pos.y][pos.x] = pnj.get();
    }

    for (auto& obj : m_objs) {
        sf::Vector2i pos = sf::Vector2i(obj->getPosition().x, obj->getPosition().y);
        if (pos.y >= 0 && pos.y < (int)m_height && pos.x >= 0 && pos.x < (int)m_width)
            m_interactables[pos.y][pos.x] = obj.get();
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

void Zone::drawAll(sf::RenderWindow& window, const EntiteMonde& player) {
    window.draw(m_tileMap);

    // Remplacement de getGridPosition par getPosition
    sf::Vector2i playerPos = sf::Vector2i(player.getPosition().x, player.getPosition().y);

    for (const auto& pnj : m_pnjs) 
        if (pnj->getPosition().y <= playerPos.y) pnj->draw(window);
    
    for (const auto& obj : m_objs) 
        if (obj->getPosition().y <= playerPos.y) obj->draw(window);

    player.draw(window);

    for (const auto& pnj : m_pnjs) 
        if (pnj->getPosition().y > playerPos.y) pnj->draw(window);
    
    for (const auto& obj : m_objs) 
        if (obj->getPosition().y > playerPos.y) obj->draw(window);
}

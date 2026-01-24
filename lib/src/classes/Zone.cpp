#include <iostream>
#include <fstream>
#include "Zone.hpp"
#include "EntiteMonde.hpp"

const int TILE_SIZE = 32;

Zone::Zone(int id,
           std::vector<std::unique_ptr<Pnj>> pnjs,
           std::vector<std::unique_ptr<Obj>> objs,
           TileMap tileMap)
    : m_id(id)
    , m_pnjs(std::move(pnjs))
    , m_objs(std::move(objs))
    , m_tileMap(std::move(tileMap))
{
    // --- collisionMap ---
    std::ifstream file("assets/zone/zone" + std::to_string(m_id) + "/collisionMap.txt");
    if (!file) return;

    std::string line;
    while (std::getline(file, line))
        m_collisionMap.push_back(line);

    // --- visualMap ---
    std::ifstream visFile("assets/zone/zone" + std::to_string(m_id) + "/map.txt");
    if (!visFile) return;

    while (std::getline(visFile, line))
        m_visualMap.push_back(line);

    // --- TileMap ---
    m_tileMap.load(
        "assets/zone/zone" + std::to_string(m_id) + "/tileset.png",
        sf::Vector2u(TILE_SIZE, TILE_SIZE),
        m_visualMap,
        m_visualMap[0].size(),
        m_visualMap.size()
    );

    // --- Grille interactable ---
    m_interactables.resize(m_visualMap.size(),
        std::vector<Interactable*>(m_visualMap[0].size(), nullptr));

    for (auto& pnj : m_pnjs) {
        auto pos = pnj->getPosition();
        m_interactables[pos.y][pos.x] = pnj.get();
    }

    for (auto& obj : m_objs) {
        auto pos = obj->getPosition();
        m_interactables[pos.y][pos.x] = obj.get();
    }
}

// --- Accesseurs simples ---
int Zone::getId() const {
    return m_id;
}

std::vector<std::string> Zone::getCollisionMap() const {
    return m_collisionMap;
}

std::vector<std::unique_ptr<Pnj>>& Zone::getPnjs() {
    return m_pnjs;
}

std::vector<std::unique_ptr<Obj>>& Zone::getObjs() {
    return m_objs;
}

const std::vector<ZoneTransition>& Zone::getTransitions() const {
    return m_transitions;
}

// --- Dessin de la zone ---
void Zone::drawAll(sf::RenderWindow& window, const EntiteMonde& player)
{   
    window.draw(m_tileMap);

    // 1. Dessin des entités "au-dessus" du joueur
    for (const auto& pnj : m_pnjs)
        if (pnj->getPosition().y <= player.getPosition().y)
            pnj->draw(window);

    for (const auto& obj : m_objs)
        if (obj->getPosition().y <= player.getPosition().y)
            obj->draw(window);

    // 2. Dessin du joueur
    player.draw(window);

    // 3. Dessin des entités "en-dessous" du joueur
    for (const auto& pnj : m_pnjs)
        if (pnj->getPosition().y > player.getPosition().y)
            pnj->draw(window);

    for (const auto& obj : m_objs)
        if (obj->getPosition().y > player.getPosition().y)
            obj->draw(window);
}

// --- Accès aux interactables ---
Interactable* Zone::getInteractableAt(int x, int y) const
{
    if (x < 0 || y < 0
        || y >= (int)m_interactables.size()
        || x >= (int)m_interactables[0].size())
    {
        return nullptr;
    }

    return m_interactables[y][x];
}

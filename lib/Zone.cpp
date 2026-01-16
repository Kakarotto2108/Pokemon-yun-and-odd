#include <iostream>
#include <fstream>
#include "Zone.hpp"
#include "Player.hpp"
#include "EntiteMonde.hpp"

const int TILE_SIZE = 32;

Zone::Zone(const int id, std::vector<std::unique_ptr<Pnj>> pnjs, std::vector<std::unique_ptr<Obj>> objs, TileMap tileMap)
    : m_id(id), m_pnjs(std::move(pnjs)), m_objs(std::move(objs)), m_tileMap(std::move(tileMap)) {

    std::ifstream file("assets/zone/zone" + std::to_string(m_id) + "/collisionMap.txt");
     if (!file) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier de collision pour la zone " << m_id << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        m_collisionMap.push_back(line);
    }
    file.close();

    // Chargement de la carte visuelle
    std::ifstream visFile("assets/zone/zone" + std::to_string(m_id) + "/map.txt");
    if (!visFile) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier de carte visuelle pour la zone " << m_id << std::endl;
        return;
    }
    std::string visLine;
    while (std::getline(visFile, visLine)) {
        m_visualMap.push_back(visLine);
    }
    visFile.close();

    if (!m_tileMap.load("assets/zone/zone" + std::to_string(m_id) + "/tileset.png", sf::Vector2u(TILE_SIZE, TILE_SIZE), m_visualMap, m_visualMap[0].size(), m_visualMap.size())) {
        std::cerr << "Erreur chargement TileMap\n";
        return;
    }

    if (m_id == 1) {
        m_transitions.push_back(ZoneTransition{2, sf::Vector2i(5,0)});
    } else if (m_id == 2) {
        m_transitions.push_back(ZoneTransition{1, sf::Vector2i(5,6)});
    }
    }

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

void Zone::drawAll(sf::RenderWindow& window, const EntiteMonde& player)
{   
    window.draw(m_tileMap);
    // 1. On dessine tout ce qui est AU-DESSUS du joueur
    for (const auto& pnj : m_pnjs) {
        if (pnj->getPosition().y <= player.getPosition().y) {
            pnj->draw(window);
        }
    }

    for (const auto& obj : m_objs) {
        if (obj->getPosition().y <= player.getPosition().y) {
            obj->draw(window);
        }
    }

    // 2. Puis le joueur
    player.draw(window);

    // 3. Puis tout ce qui est EN-DESSOUS du joueur
    for (const auto& pnj : m_pnjs) {
        if (pnj->getPosition().y > player.getPosition().y) {
            pnj->draw(window);
        }
    }

    // 4. Puis tout ce qui est EN-DESSOUS du joueur
    for (const auto& obj : m_objs) {
        if (obj->getPosition().y > player.getPosition().y) {
            obj->draw(window);
        }
    }
}
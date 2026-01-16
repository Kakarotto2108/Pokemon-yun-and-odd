#include <iostream>
#include <fstream>
#include "Zone.hpp"
#include "Player.hpp"
#include "EntiteMonde.hpp"

const int TILE_SIZE = 32;

Zone::Zone(const int id,
           std::vector<std::unique_ptr<Pnj>> pnjs,
           std::vector<std::unique_ptr<Obj>> objs,
           TileMap tileMap)
    : m_id(id), m_pnjs(std::move(pnjs)), m_objs(std::move(objs)), m_tileMap(std::move(tileMap))
{
    // --- Chargement collisionMap ---
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

    // --- Chargement visualMap ---
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

    // --- Chargement TileMap ---
    if (!m_tileMap.load("assets/zone/zone" + std::to_string(m_id) + "/tileset.png",
                        sf::Vector2u(TILE_SIZE, TILE_SIZE),
                        m_visualMap,
                        m_visualMap[0].size(),
                        m_visualMap.size()))
    {
        std::cerr << "Erreur chargement TileMap\n";
        return;
    }

    // --- Initialisation de la grille interactable ---
    m_interactables.resize(m_visualMap.size()); // lignes = hauteur
    for (auto& row : m_interactables)
        row.resize(m_visualMap[0].size(), nullptr); // colonnes = largeur

    // --- Placement des PNJ dans la grille ---
    for (auto& pnj : m_pnjs) {
        sf::Vector2i pos = pnj->getPosition();
        if (pos.y >= 0 && pos.y < (int)m_interactables.size() &&
            pos.x >= 0 && pos.x < (int)m_interactables[0].size())
        {
            m_interactables[pos.y][pos.x] = pnj.get();
        }
    }

    // --- Placement des Objets dans la grille ---
    for (auto& obj : m_objs) {
        sf::Vector2i pos = obj->getPosition();
        if (pos.y >= 0 && pos.y < (int)m_interactables.size() &&
            pos.x >= 0 && pos.x < (int)m_interactables[0].size())
        {
            m_interactables[pos.y][pos.x] = obj.get();
        }
    }

    for (auto& pnjPtr : currentZone.getPnjs())
    {
        Pnj* pnj = pnjPtr.get();
        pnj->onDialogue.subscribe([this](const std::string& dialogue){
            m_messageBox.setText(dialogue);
            m_messageBox.show();
        });

        pnj->onItemGiven.subscribe([this](const Item& item){
            m_messageBox.setObj(item.getName());
            m_messageBox.hasObj();
            m_messageBox.show();
        });
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

#include "World.hpp"
#include <iostream>

Monde::Monde() {}

void Monde::addZone(std::unique_ptr<Zone> zone) {
    int id = zone->getId();
    m_zones[id] = std::move(zone); // maintenant sûr
}


Zone& Monde::getZoneActuelle() {
    return *m_zones.at(m_currentZoneId);
}

void Monde::changerZone(int id) {
    m_currentZoneId = id;
    // repositionnement du joueur fait par Jeu
}

int Monde::getCurrentZoneId() const {
    return m_currentZoneId;
}

void Monde::draw(sf::RenderWindow& window, const EntiteMonde& focus)
{
    getZoneActuelle().drawAll(window, focus);
}

void Monde::init()
{
    std::vector<std::unique_ptr<Pnj>> pnjs;
    std::vector<std::unique_ptr<Obj>> objects;

    // Création des objets
    Item pokeball("Poké ball", ItemPocket::Balls, "Un objet semblable à une capsule.", true);

    // Exemple PNJ
    pnjs.push_back(std::make_unique<Pnj>(
        "assets/sprite/pnj/48049.png",
        "assets/sprite/pnj/48050.png",
        sf::Vector2i(4,2), 0,
        "Bonjour !",
        pokeball
    ));

    // Exemple objet
    objects.push_back(std::make_unique<Obj>(
        "assets/sprite/obj/IMG_1338.png",
        sf::Vector2i(5,0),
        "Cette télé est un cadeau.",
        pokeball
    ));

    // Zone 1
    auto zone1 = std::make_unique<Zone>(1, std::move(pnjs), std::move(objects));
    zone1->m_transitions.push_back({2, sf::Vector2i(5,0)});
    addZone(std::move(zone1));

    auto zone2 = std::make_unique<Zone>(
    2,
    std::vector<std::unique_ptr<Pnj>>{},
    std::vector<std::unique_ptr<Obj>>{}
    );

    zone2->m_transitions.push_back({1, sf::Vector2i(5,6)});
    addZone(std::move(zone2));

    m_currentZoneId = 1;
}




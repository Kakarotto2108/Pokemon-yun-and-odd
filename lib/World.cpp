#include "Monde.hpp"

Monde::Monde() : m_zoneActuelleId(1) {}

void Monde::addZone(std::unique_ptr<Zone> zone) {
    m_zones[zone->getId()] = std::move(zone);
}

Zone& Monde::getZoneActuelle() {
    return *m_zones.at(m_zoneActuelleId);
}

void Monde::changerZone(int id, sf::Vector2i nouvellePos) {
    m_zoneActuelleId = id;
    // repositionnement du joueur fait par Jeu
}

void Monde::draw(sf::RenderWindow& window, Player& player) {
    getZoneActuelle().drawAll(window, player);
}

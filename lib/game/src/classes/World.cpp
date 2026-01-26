#include "World.hpp"
#include "ZoneFactory.hpp"
#include <iostream>

World::World() : m_currentZoneId(1) {}

void World::addZone(std::unique_ptr<Zone> zone) {
    if (zone) {
        int id = zone->getId();
        m_zones[id] = std::move(zone);
    }
}

Zone& World::getZoneActuelle() {
    return *m_zones.at(m_currentZoneId);
}

void World::changerZone(int id) {
    if (m_zones.find(id) != m_zones.end()) {
        m_currentZoneId = id;
    }
}

int World::getCurrentZoneId() const {
    return m_currentZoneId;
}

void World::draw(sf::RenderWindow& window, const WorldEntity& focus) {
    getZoneActuelle().drawAll(window, focus);
}

void World::init() {
    addZone(ZoneFactory::createZone(1));
    addZone(ZoneFactory::createZone(2));

    m_currentZoneId = 1;
}

void World::update(Player& player) {
    // 1. Récupérer la zone actuelle
    Zone& zone = getZoneActuelle();
    sf::Vector2i pos = player.getPosition();

    // 2. Vérifier si la case actuelle est une case de collision/transition
    int tileValue = zone.getCollisionMap()[pos.x + pos.y * zone.getWidth()];

    // 3. Si la valeur correspond à une transition
    if (tileValue > 0) {
        int targetZoneId = math::floor(tileValue);
        int targetSpawnIndex = tileValue - targetZoneId;

        // Logique de changement de zone
        changerZone(targetZoneId);
            
        // On place le joueur à sa nouvelle position
        player.setLogicalPos(m_zones[targetZoneId]->getSpawnPos(targetSpawnIndex));
    }
}

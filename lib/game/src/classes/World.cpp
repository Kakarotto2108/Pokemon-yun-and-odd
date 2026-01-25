#include "World.hpp"
#include "ZoneFactory.hpp"
#include <iostream>

Monde::Monde() : m_currentZoneId(1) {}

void Monde::addZone(std::unique_ptr<Zone> zone) {
    if (zone) {
        int id = zone->getId();
        m_zones[id] = std::move(zone);
    }
}

Zone& Monde::getZoneActuelle() {
    return *m_zones.at(m_currentZoneId);
}

void Monde::changerZone(int id) {
    if (m_zones.find(id) != m_zones.end()) {
        m_currentZoneId = id;
    }
}

int Monde::getCurrentZoneId() const {
    return m_currentZoneId;
}

void Monde::draw(sf::RenderWindow& window, const WorldEntity& focus) {
    getZoneActuelle().drawAll(window, focus);
}

void Monde::init() {
    addZone(ZoneFactory::createZone(1));
    addZone(ZoneFactory::createZone(2));

    m_currentZoneId = 1;
}
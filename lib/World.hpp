#pragma once
#include <memory>
#include <unordered_map>
#include "Zone.hpp"

class Monde {
public:
    Monde();

    void addZone(std::unique_ptr<Zone> zone);
    Zone& getZoneActuelle();
    void changerZone(int id, sf::Vector2i nouvellePos);

    void draw(sf::RenderWindow& window, Player& player);

private:
    std::unordered_map<int, std::unique_ptr<Zone>> m_zones;
    int m_zoneActuelleId;
};

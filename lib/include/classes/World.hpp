#pragma once

#include <vector>
#include <memory>
#include "Zone.hpp"

class Monde {
public:
    Monde();

    void addZone(std::unique_ptr<Zone> zone);
    Zone& getZoneActuelle();
    void changerZone(int zoneId);
    int getCurrentZoneId() const;

    void update(Player& player);
    void draw(sf::RenderWindow& window, const EntiteMonde& focus);
    void init();

private:
    std::map<int,std::unique_ptr<Zone>> m_zones; 
    int m_currentZoneId;
};

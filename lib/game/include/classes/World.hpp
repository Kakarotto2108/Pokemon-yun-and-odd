#pragma once

#include <vector>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

#include "Zone.hpp"
#include "WorldEntity.hpp"
#include "Player.hpp"

class Player;

class World {
public:
    World();

    // Responsabilité : Gestion de la collection de zones
    void addZone(std::unique_ptr<Zone> zone);
    
    // Accesseurs
    Zone& getZoneActuelle();
    int getCurrentZoneId() const;
    
    // Responsabilité : Contrôleur d'état du monde
    void changerZone(int zoneId);
    
    // Orchestration (Délègue à la Zone actuelle - Information Expert)
    void update(Player& player);
    void draw(sf::RenderWindow& window, const WorldEntity& focus);
    
    // Initialisation via Factory (L'implémentation utilisera ZoneFactory)
    void init();

private:
    // Stockage des zones identifiées par leur ID
    std::map<int, std::unique_ptr<Zone>> m_zones; 
    int m_currentZoneId;
};
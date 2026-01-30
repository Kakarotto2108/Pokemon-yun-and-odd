#pragma once

#include <vector>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

#include "Zone.hpp"
#include "WorldEntity.hpp"
#include "Player.hpp"
#include "Character.hpp"
#include "Obj.hpp"
#include "ItemGround.hpp"

class Player;

class World {
public:
    // ---- Singleton access ----
    static World& getInstance() {
        static World instance;
        return instance;
    }


    // Responsabilité : Gestion de la collection de zones
    void addZone(std::unique_ptr<Zone> zone);

    // Accesseurs
    Zone& getCurrentZone();
    int getCurrentZoneId() const;

    // Responsabilité : Contrôleur d'état du monde
    void switchZone(int zoneId);

    // Orchestration
    void update(float dt, Player& player);
    void draw(sf::RenderWindow& window, const WorldEntity& focus);
    void drawTileMap3D(const TileMap& tileMap);
    void drawCharacter3D(const Character& character);
    void drawObjSprite3D(const Obj& obj);
    void drawIogSprite3D(const Iog& iog);
    void renderEntities(Zone& zone);
    void draw3D(sf::RenderTarget& target);
    void destroyEntity(WorldEntity* entity);

    // Initialisation via Factory
    void init();

private:
    World() = default;
    
    // Supprime copie / assignation
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    std::unique_ptr<Zone> m_zone;
};

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Pnj.hpp"
#include "TileMap.hpp"
#include "Obj.hpp"


class Player;  // 👈 déclaration anticipée

struct ZoneTransition {
    int targetZoneId;        // ID de la zone cible
    sf::Vector2i spawnPos;   // Position du joueur dans la nouvelle zone
};

class Zone {
public:
    Zone(const int id,
        std::vector<std::unique_ptr<Pnj>> pnjs, 
        std::vector<std::unique_ptr<Obj>> objs,
        TileMap tileMap = TileMap()
        );

    int getId() const;
    std::vector<std::string> getCollisionMap() const;
    std::vector<std::unique_ptr<Pnj>>& getPnjs();   
    std::vector<std::unique_ptr<Obj>>& getObjs();
    sf::Vector2i getPlayerStartPos() const;
    void drawAll(sf::RenderWindow& window, Player& player);
    const std::vector<ZoneTransition>& getTransitions() const;

    std::vector<std::string> m_collisionMap;

private:
    int m_id;
    std::vector<std::unique_ptr<Pnj>> m_pnjs;
    std::vector<std::unique_ptr<Obj>> m_objs;
    std::vector<std::string> m_visualMap;
    TileMap m_tileMap;
    std::vector<ZoneTransition> m_transitions;  
};

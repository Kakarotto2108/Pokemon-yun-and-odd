#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

#include "EntiteMonde.hpp"   // ✅ MANQUANT
#include "Pnj.hpp"
#include "TileMap.hpp"
#include "Obj.hpp"

class Player;  // OK si utilisé ailleurs (pas nécessaire ici)

struct ZoneTransition {
    int targetZoneId;
    sf::Vector2i spawnPos;
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

    void drawAll(sf::RenderWindow& window, const EntiteMonde& player);

    const std::vector<ZoneTransition>& getTransitions() const;
    Interactable* getInteractableAt(int x, int y) const;

    std::vector<std::string> m_collisionMap;
    std::vector<ZoneTransition> m_transitions; 

private:
    int m_id;
    std::vector<std::unique_ptr<Pnj>> m_pnjs;
    std::vector<std::unique_ptr<Obj>> m_objs;
    std::vector<std::string> m_visualMap;
    TileMap m_tileMap; 
    std::vector<std::vector<Interactable*>> m_interactables;

};

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "TileMap.hpp"
#include "Pnj.hpp"
#include "Obj.hpp"
#include "Interactable.hpp"

struct ZoneTransition {
    int targetZoneId;
    sf::Vector2i targetSpawnPos;
};

class Zone {
public:
    Zone(int id, 
         std::vector<std::unique_ptr<Pnj>> pnjs, 
         std::vector<std::unique_ptr<Obj>> objs);

    void drawAll(sf::RenderWindow& window, const class EntiteMonde& player);
    
    int getId() const { return m_id; }
    bool isBlocking(int x, int y) const;
    Interactable* getInteractableAt(int x, int y) const;
    
    // Ajoute cet accesseur pour corriger l'erreur dans Game.cpp
    std::vector<ZoneTransition>& getTransitions() { return m_transitions; }
    const std::vector<int>& getCollisionMap() const { return m_collisionMap; }
    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }

private:
    int m_id;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    
    TileMap m_tileMap;
    std::vector<int> m_visualMap;
    std::vector<int> m_collisionMap;
    
    std::vector<std::unique_ptr<Pnj>> m_pnjs;
    std::vector<std::unique_ptr<Obj>> m_objs;
    std::vector<ZoneTransition> m_transitions;
    std::vector<std::vector<Interactable*>> m_interactables;

    void loadData();
    void updateInteractableGrid();
};
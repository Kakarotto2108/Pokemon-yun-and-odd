#pragma once
#include "World.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "Zone.hpp"

class PlayerController {
public:
    PlayerController(World& world, Player& player);
    void update(Zone& zone, float dt);

private:
    World& m_world;
    Player& m_player;
    float m_hAxis = 0.f;
    float m_vAxis = 0.f;
    
    sf::Clock m_moveTimer;
};
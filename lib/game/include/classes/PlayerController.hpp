#pragma once
#include "Player.hpp"
#include "Controller.hpp"
#include "Zone.hpp"

class PlayerController {
public:
    PlayerController(Player& player);
    void update(Zone& zone, float dt);

private:
    Player& m_player;
    float m_hAxis = 0.f;
    float m_vAxis = 0.f;
    
    sf::Clock m_moveTimer;
    const float m_moveDelay = 0.2f; // Vitesse de répétition du pas
};
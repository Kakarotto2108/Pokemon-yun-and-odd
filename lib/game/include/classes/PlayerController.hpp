#pragma once
#include "World.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "Zone.hpp"

class PlayerController {
public:
    static PlayerController* getInstance();
    static void create(World& world, Player& player);
    static void destroy();

    void update(Zone& zone, float dt);
    void enableInput() { m_inputEnabled = true; }
    void disableInput() { m_inputEnabled = false; }

    PlayerController(const PlayerController&) = delete;
    PlayerController& operator=(const PlayerController&) = delete;

private:
    PlayerController(World& world, Player& player);
    static PlayerController* s_instance;

    World& m_world;
    Player& m_player;
    float m_hAxis = 0.f;
    float m_vAxis = 0.f;
    
    sf::Clock m_moveTimer;
    bool m_inputEnabled = true;
};
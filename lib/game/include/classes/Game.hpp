#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Config.hpp"

#include "Player.hpp"
#include "PlayerController.hpp"
#include "World.hpp"

class Game {
public:
    Game(const GameConfig& config);
    void run();

private:
    void handleEvents();
    void update(float dt);
    void render();

private:
    sf::RenderWindow m_window;
    sf::View m_cameraView;
    sf::View m_uiView;

    Player m_player;
    std::unique_ptr<PlayerController> m_playerController;
    World m_world;
};

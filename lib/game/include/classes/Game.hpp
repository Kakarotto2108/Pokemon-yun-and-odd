#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Config.hpp"

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
    GameConfig m_config;
    sf::RenderWindow m_window;
    Player& m_player;
    sf::View m_cameraView;
    sf::View m_uiView;

    std::unique_ptr<PlayerController> m_playerController;
    World m_world;
};

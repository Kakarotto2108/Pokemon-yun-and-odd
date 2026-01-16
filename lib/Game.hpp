#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Player.hpp"
#include "World.hpp"
#include "MessageBox.hpp"

class Jeu {
public:
    Jeu();
    void run();

private:
    void handleEvents();
    void update();
    void render();

private:
    sf::RenderWindow m_window;
    sf::View m_cameraView;
    sf::View m_uiView;

    Player m_player;
    Monde m_world;
    MessageBox m_messageBox;

    sf::Clock m_moveClock;
    const float m_delayTime = 0.15f;
};

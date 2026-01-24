#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Player.hpp"
#include "PlayerController.hpp"
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
    bool hasSeenZero = false;
    bool m_ePressedLastFrame = false;


    Player m_player;
    std::unique_ptr<PlayerController> m_playerController;
    Monde m_world;
    MessageBox m_messageBox;
};

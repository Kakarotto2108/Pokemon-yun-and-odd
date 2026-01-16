#include "Game.hpp"

Jeu::Jeu()
    : m_window(sf::VideoMode(640,480), "Pokemon"),
      m_player("face.png","l.png","r.png","up.png","Kakarot"),
      m_messageBox(sf::Vector2f(600.f, 90.f), sf::Vector2f(-5.f, 8.f))
{
    m_window.setFramerateLimit(120);
    m_world.init();
}

// run()
void Jeu::run() {
    while (m_window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

// update()
void Jeu::update()
{
    if (m_moveClock.getElapsedTime().asSeconds() >= m_delayTime) {
        m_player.handleInput(m_window, m_world.getZoneActuelle(), m_delayTime, m_messageBox);
        m_moveClock.restart();
    }

    m_player.update();

    m_cameraView.setCenter(m_player.getDrawPosition());
}

void Jeu::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
    }
}

void Jeu::render()
{
    m_window.clear();

    m_world.draw(m_window, m_player);

    m_window.display();
}

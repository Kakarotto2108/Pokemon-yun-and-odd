#include "Game.hpp"
#include "DialogManager.hpp"
#include <iostream>

Game::Game(const GameConfig& config)
    : m_window(sf::VideoMode(config.width, config.height), config.title)
    , m_player(Player::getInstance()) // Le constructeur de Player chargera ses assets via ResourceManager en interne
{
    m_config = config;
    m_window.setFramerateLimit(120);

    // Caméra "Game"
    m_cameraView.setSize(static_cast<sf::Vector2f>(m_window.getSize())); // taille = fenêtre
    m_cameraView.setCenter(m_player.getDrawPosition()); // centre sur le joueur

    // Vue UI (toujours fixe)
    m_uiView.setSize(static_cast<sf::Vector2f>(m_window.getSize()));
    m_uiView.setCenter(static_cast<sf::Vector2f>(m_window.getSize())/2.f);

    m_world.init();

    m_playerController = std::make_unique<PlayerController>(m_world,m_player); // À ajouter avant l'accolade de fin du constructeur
}


void Game::run() {
    sf::Clock clock;

    MessageBox dialogUI;
    dialogUI.setPosition({0.f, m_config.height - 135.f}); // Position par défaut en bas de l'écran
    DialogManager::getInstance().init(&dialogUI);

    while (m_window.isOpen()) {
        sf::Time dt = clock.restart(); // On récupère le temps écoulé
        
        Controller::getInstance().handleInput(m_window);
        handleEvents();
        
        // On passe le temps réel à l'update
        update(dt.asSeconds()); 
        render();
    }
}

void Game::update(float dt)
{
    m_playerController->update(m_world.getZoneActuelle(), dt);
    
    m_world.update(m_player); 
    
    m_player.update(dt);
    m_cameraView.setCenter(m_player.getDrawPosition());
}

void Game::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();
    }
}

void Game::render()
{
    m_window.clear();

    // --- Monde ---
    m_window.setView(m_cameraView);
    m_world.draw(m_window, m_player);

    // --- UI ---
    m_window.setView(m_uiView);
    DialogManager::getInstance().draw(m_window);

    m_window.display();
}

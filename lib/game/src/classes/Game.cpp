#include "Game.hpp"
#include "DialogManager.hpp"
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    
    GameDialog dialogUI;
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
    m_playerController->update(m_world.getCurrentZone(), dt);
    
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

void Game::render() {
    m_window.clear(sf::Color::Black);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    sf::Vector2f pPos = m_player.getDrawPosition();

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.f), 
        (float)m_config.width / (float)m_config.height, 
        0.1f, 
        3000.f
    );

    glm::vec3 cameraPos = glm::vec3(pPos.x, pPos.y + 200.f, -400.f); 
    glm::vec3 cameraTarget = glm::vec3(pPos.x, pPos.y, 0.f);

    glm::mat4 viewMatrix = glm::lookAt(
        cameraPos,
        cameraTarget,
        glm::vec3(0.f, -1.f, 0.f) // On garde le Up inversé pour l'axe Y de SFML
    );

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projectionMatrix));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(viewMatrix));

    // 2. Setup States
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING); // IMPORTANT pour éviter le noir
    glColor4f(1.f, 1.f, 1.f, 1.f); // Force le blanc pour le mode "Unlit"

    m_world.draw3D(m_window);

    m_window.pushGLStates();
    m_window.setView(m_uiView);
    DialogManager::getInstance().draw(m_window);
    m_window.popGLStates();

    m_window.display();

    // m_window.clear(sf::Color::Black);
    // // --- Monde ---
    // m_window.setView(m_cameraView);
    // m_world.draw(m_window, m_player);

    // // --- UI ---
    // m_window.setView(m_uiView);
    // DialogManager::getInstance().draw(m_window);

    // m_window.display();
}

#include "Game.hpp"
#include "DialogManager.hpp"
#include "TransitionManager.hpp"
#include "Event.hpp"
#include "Menu.hpp"
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

    if (!m_sceneBuffer.create(config.width, config.height, sf::ContextSettings(24))) { // 24 bits pour le Depth Buffer
        std::cerr << "Erreur : Impossible de créer m_sceneBuffer" << std::endl;
    }
    TransitionManager::getInstance().init(m_window.getSize());

    // Caméra "Game"
    m_cameraView.setSize(static_cast<sf::Vector2f>(m_window.getSize())); // taille = fenêtre
    m_cameraView.setCenter(m_player.getDrawPosition()); // centre sur le joueur

    // Vue UI (toujours fixe)
    m_uiView.setSize(static_cast<sf::Vector2f>(m_window.getSize()));
    m_uiView.setCenter(static_cast<sf::Vector2f>(m_window.getSize())/2.f);

    World::getInstance().init();

    PlayerController::create(World::getInstance(), m_player);
    m_playerController = PlayerController::getInstance();// À ajouter avant l'accolade de fin du constructeur
    Menu::getInstance();
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

void Game::update(float dt) {
    TransitionManager::getInstance().update(dt);

    // On bloque l'update du monde/joueur si on est en plein milieu d'une transition ? 
    // Ou on laisse tourner, selon ton choix de Game Design.
    if(!TransitionManager::getInstance().isRunning()) {
         m_playerController->update(World::getInstance().getCurrentZone(), dt);
         World::getInstance().update(dt, m_player); 
    }

    //m_cameraView.setCenter(m_player.getDrawPosition());
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
    // DEBUG: Début du cycle
    // std::cout << "[Debug] Start Render" << std::endl;

    // 1. Préparation du m_sceneBuffer
    if (!m_sceneBuffer.setActive(true)) {
        std::cerr << "[Error] Failed to set m_sceneBuffer active" << std::endl;
        return;
    }

    m_sceneBuffer.clear(sf::Color::Black);
    
    // On nettoie le buffer de profondeur (indispensable pour la 3D)
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // --- SETUP OPENGL 3D ---
    // On sauvegarde les états de SFML avant d'attaquer l'OpenGL pur
    // std::cout << "[Debug] Drawing 3D World..." << std::endl;
    
    glPushAttrib(GL_ALL_ATTRIB_BITS); // Sauvegarde tous les états actuels

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor4f(1.f, 1.f, 1.f, 1.f);

    sf::Vector2f pPos = m_player.getDrawPosition();
    
    // Matrices
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.f), 
        (float)m_config.width / (float)m_config.height, 
        0.1f, 
        3000.f
    );
    
    glm::vec3 cameraPos = glm::vec3(pPos.x, pPos.y + 200.f, -400.f); 
    glm::vec3 cameraTarget = glm::vec3(pPos.x, pPos.y, 0.f);
    glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.f, -1.f, 0.f));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projectionMatrix));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(viewMatrix));

    // Appel au rendu du monde
    World::getInstance().draw3D(m_sceneBuffer); 

    glPopAttrib(); // On restaure les états OpenGL tels qu'ils étaient

    m_sceneBuffer.display();
    m_sceneBuffer.setActive(false); // TRÈS IMPORTANT

    // On s'assure que la fenêtre reprend le contrôle
    m_window.setActive(true);
    m_window.clear(sf::Color::Black);

    // On dessine le Sprite de base SANS Shaders d'abord
    m_window.pushGLStates(); // Sauvegarde les états OpenGL de ton monde 3D
    
    sf::Sprite sceneSprite(m_sceneBuffer.getTexture());
    m_window.draw(sceneSprite);
    
    // On dessine la transition
    TransitionManager::getInstance().render(m_window, m_sceneBuffer.getTexture());

    // On dessine l'UI
    m_window.setView(m_uiView);
    DialogManager::getInstance().draw(m_window);

    m_window.popGLStates(); // Restaure les états pour le prochain tour
    m_window.display();
}

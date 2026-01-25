#include "Game.hpp"
#include <iostream>

Game::Game(const GameConfig& config)
    : m_window(sf::VideoMode(config.width, config.height), config.title)
    , m_player(config.playerName) // Le constructeur de Player chargera ses assets via ResourceManager en interne
{
    m_window.setFramerateLimit(120);

    // Caméra "Game"
    m_cameraView.setSize(static_cast<sf::Vector2f>(m_window.getSize())); // taille = fenêtre
    m_cameraView.setCenter(m_player.getDrawPosition()); // centre sur le joueur

    // Vue UI (toujours fixe)
    m_uiView.setSize(static_cast<sf::Vector2f>(m_window.getSize()));
    m_uiView.setCenter(static_cast<sf::Vector2f>(m_window.getSize())/2.f);

    m_world.init();

    m_playerController = std::make_unique<PlayerController>(m_player); // À ajouter avant l'accolade de fin du constructeur
}


void Game::run() {
    sf::Clock clock;
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
    
    m_player.update();
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

    m_window.display();
}


 // // Récupérer la position logique du joueur
    // sf::Vector2i pos = m_player.getPosition();
    // Zone& currentZone = m_world.getZoneActuelle();
    // const auto& transitions = currentZone.getTransitions();

    // // Accès 1D : index = x + y * largeur
    // int tile = currentZone.getCollisionMap()[pos.x + pos.y * currentZone.getWidth()];

    // if (tile > 0) 
    // {
    //     auto it = std::find_if(transitions.begin(), transitions.end(),
    //         [tile](const ZoneTransition& t){ return t.targetZoneId == tile; });

    //     if (it != transitions.end() && hasSeenZero) {
    //         m_world.changerZone(it->targetZoneId);
    //         m_player.setLogicalPos(it->targetSpawnPos);
    //         hasSeenZero = false;
    //     }
    // } 
    // else 
    // {
    //     hasSeenZero = true;
    // }
    // bool ePressedNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);

    // if (ePressedNow && !m_ePressedLastFrame)  // Détection du "front montant"
    // {
    //     sf::Vector2i front = m_player.getFacingTile();
    //     Zone& currentZone = m_world.getZoneActuelle();
    //     Interactable* interactable = currentZone.getInteractableAt(front.x, front.y);

    //     if (interactable)
    //     {
    //         if (auto item = interactable->giveItem()) {
    //             m_messageBox.setPendingItem(*item);
    //             m_messageBox.setObj(item->getName());
    //         }
    //         interactable->interact();
    //         m_messageBox.nextPage(m_window, interactable->getDialogue());
    //     }
    // }    

    // // Sauvegarder l'état pour la prochaine frame
    // m_ePressedLastFrame = ePressedNow;
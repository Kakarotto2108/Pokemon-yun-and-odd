#include "Game.hpp"
#include "Controller.hpp"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(640,480), "Pokemon"),
      m_player("./assets/sprite/pnj/player_face.png","./assets/sprite/pnj/player_l.png","./assets/sprite/pnj/player_r.png","./assets/sprite/pnj/player_up.png","Kakarot"),
      m_messageBox(sf::Vector2f(600.f, 90.f), sf::Vector2f(-5.f, 8.f))
{
    m_window.setFramerateLimit(120);

    // Caméra "Game"
    m_cameraView.setSize(static_cast<sf::Vector2f>(m_window.getSize())); // taille = fenêtre
    m_cameraView.setCenter(m_player.getDrawPosition()); // centre sur le joueur

    // Vue UI (toujours fixe)
    m_uiView.setSize(static_cast<sf::Vector2f>(m_window.getSize()));
    m_uiView.setCenter(static_cast<sf::Vector2f>(m_window.getSize())/2.f);

    m_world.init();

    m_messageBox.onItemGiven.subscribe([this](const Item& item){
        m_player.receiveItem(item); // ajoute l'objet au joueur
    });

    m_playerController = std::make_unique<PlayerController>(m_player); // À ajouter avant l'accolade de fin du constructeur
}


// run()
void Game::run() {
    while (m_window.isOpen()) {
        Controller::getInstance().handleInput(m_window);
        handleEvents();
        update();
        render();
    }
}

// update()
void Game::update()
{
    m_playerController->update(m_world.getZoneActuelle(), 0.f);

    // Update du joueur (animation, etc.)
    m_player.update();

    // Récupérer la position logique du joueur
    sf::Vector2i pos = m_player.getPosition();
    Zone& currentZone = m_world.getZoneActuelle();
    const auto& transitions = currentZone.getTransitions();

    // Accès 1D : index = x + y * largeur
    int tile = currentZone.getCollisionMap()[pos.x + pos.y * currentZone.getWidth()];

    if (tile > 0) 
    {
        auto it = std::find_if(transitions.begin(), transitions.end(),
            [tile](const ZoneTransition& t){ return t.targetZoneId == tile; });

        if (it != transitions.end() && hasSeenZero) {
            m_world.changerZone(it->targetZoneId);
            m_player.setLogicalPos(it->targetSpawnPos);
            hasSeenZero = false;
        }
    } 
    else 
    {
        hasSeenZero = true;
    }
    bool ePressedNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);

    if (ePressedNow && !m_ePressedLastFrame)  // Détection du "front montant"
    {
        sf::Vector2i front = m_player.getFacingTile();
        Zone& currentZone = m_world.getZoneActuelle();
        Interactable* interactable = currentZone.getInteractableAt(front.x, front.y);

        if (interactable)
        {
            if (auto item = interactable->giveItem()) {
                m_messageBox.setPendingItem(*item);
                m_messageBox.setObj(item->getName());
            }
            interactable->interact();
            m_messageBox.nextPage(m_window, interactable->getDialogue());
        }
    }    

    // Sauvegarder l'état pour la prochaine frame
    m_ePressedLastFrame = ePressedNow;


    // Caméra suit le joueur
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
    m_messageBox.draw(m_window, m_player.m_name);

    m_window.display();
}

#include "Game.hpp"
#include <iostream>

Jeu::Jeu()
    : m_window(sf::VideoMode(640,480), "Pokemon"),
      m_player("./assets/sprite/pnj/player_face.png","./assets/sprite/pnj/player_l.png","./assets/sprite/pnj/player_r.png","./assets/sprite/pnj/player_up.png","Kakarot"),
      m_messageBox(sf::Vector2f(600.f, 90.f), sf::Vector2f(-5.f, 8.f))
{
    m_window.setFramerateLimit(120);

    // Caméra "jeu"
    m_cameraView.setSize(static_cast<sf::Vector2f>(m_window.getSize())); // taille = fenêtre
    m_cameraView.setCenter(m_player.getDrawPosition()); // centre sur le joueur

    // Vue UI (toujours fixe)
    m_uiView.setSize(static_cast<sf::Vector2f>(m_window.getSize()));
    m_uiView.setCenter(static_cast<sf::Vector2f>(m_window.getSize())/2.f);

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
    // Gestion des déplacements avec un délai
    if (m_moveClock.getElapsedTime().asSeconds() >= m_delayTime) {
        m_player.handleInput(m_window, m_world.getZoneActuelle(), m_delayTime);
        m_moveClock.restart();
    }

    // Update du joueur (animation, etc.)
    m_player.update();

    // Récupérer la position logique du joueur
    sf::Vector2i pos = m_player.getPosition();
    Zone& currentZone = m_world.getZoneActuelle();
    const auto& collisionMap = currentZone.getCollisionMap();
    const auto& transitions = currentZone.getTransitions();

    // Vérifier que le joueur est dans les limites de la map
    if (pos.y >= 0 && pos.y < (int)collisionMap.size() &&
        pos.x >= 0 && pos.x < (int)collisionMap[pos.y].size()) 
    {
        char tile = collisionMap[pos.y][pos.x];

        // Vérifier la case sur laquelle le joueur se trouve
    if (tile >= '1' && tile <= '9') {
        int targetZoneId = tile - '0';

        // Cherche la transition correspondante
        auto it = std::find_if(transitions.begin(), transitions.end(),
            [targetZoneId](const ZoneTransition& t){ return t.targetZoneId == targetZoneId; });

        if (it != transitions.end() && hasSeenZero) {
            m_world.changerZone(it->targetZoneId);
            m_player.setLogicalPos(it->spawnPos);
            hasSeenZero = false; // Réinitialiser hasSeenZero lors du changement de zone
        }
    }
        else 
        {
            // Réinitialiser hasSeenZero si le joueur n'est pas sur la tile '0'
            hasSeenZero = true;
        }
    }
    // Vérifier interaction "E"
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && !m_messageBox.isVisible())
    {
        sf::Vector2i front = m_player.getFacingTile();
        Zone& currentZone = m_world.getZoneActuelle();
        Interactable* interactable = currentZone.getInteractableAt(front.x, front.y);

        if (interactable)
        {
            // Vérifier si c'est un PNJ
            if (Pnj* pnj = dynamic_cast<Pnj*>(interactable))
            {
                pnj->onDialogue.subscribe([&](const std::string& dialogue){
                    if (m_messageBox.isVisible())
                        m_messageBox.hide(m_window);
                    else {
                        m_messageBox.setText(dialogue);
                        m_messageBox.show();
                    }
                });

                pnj->onItemGiven.subscribe([&](const Item& item){
                    if (m_messageBox.isVisible())
                        m_messageBox.hide(m_window);
                    else {
                        m_messageBox.setObj(item.getName());
                        m_messageBox.hasObj();
                        m_messageBox.show();
                    }
                });

                pnj->interact(); // Déclenche l'événement
            }
            // Vérifier si c'est un objet
            else if (Obj* obj = dynamic_cast<Obj*>(interactable))
            {
                obj->interact(); // gère l'objet si besoin
            }
        }

    }

    // Caméra suit le joueur
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

    // --- Monde ---
    m_window.setView(m_cameraView);
    m_world.draw(m_window, m_player);

    // --- UI ---
    m_window.setView(m_uiView);
    m_messageBox.draw(m_window, m_player.m_name);

    m_window.display();
}

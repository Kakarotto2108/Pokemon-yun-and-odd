#include "PlayerController.hpp"
#include "Interactable.hpp"
#include "DialogManager.hpp"
#include "TransitionManager.hpp"
#include <iostream>

PlayerController* PlayerController::s_instance = nullptr;

PlayerController* PlayerController::getInstance() {
    return s_instance;
}

void PlayerController::create(World& world, Player& player) {
    if (!s_instance)
        s_instance = new PlayerController(world, player);
}

void PlayerController::destroy() {
    delete s_instance;
    s_instance = nullptr;
}

PlayerController::PlayerController(World& world, Player& player) : m_world(world), m_player(player) {
    Controller::getInstance().onAxisChanged("MoveHorizontal", [this](float val) {
        if (DialogManager::getInstance().isActive() || TransitionManager::getInstance().isRunning()) {
            m_hAxis = 0.f;
            return;
        }
        m_hAxis = val;
    });
    Controller::getInstance().onAxisChanged("MoveVertical", [this](float val) {
        if (DialogManager::getInstance().isActive() || TransitionManager::getInstance().isRunning()) {
            m_vAxis = 0.f;
            return;
        }
        m_vAxis = val;
    });

    Controller::getInstance().onActionPressed("Interact", [this]() {
        // On bloque toute interaction pendant une transition
        if (TransitionManager::getInstance().isRunning()) return;
        
        if (DialogManager::getInstance().isActive()) {
            DialogManager::getInstance().next();
        } else {
            sf::Vector2i front = m_player.getFacingTile();
            Zone& currentZone = m_world.getCurrentZone();
            Interactable* interactable = dynamic_cast<Interactable*>(currentZone.getEntityAt(front.x, front.y));

            if (interactable)
            {
                interactable->interact();
            }
        }
    });
}

void PlayerController::update(Zone& zone, float dt) {
    if (!m_inputEnabled) return;
    if (m_moveTimer.getElapsedTime().asSeconds() < m_player.getMoveDelay() && m_player.getIsMoving()) return;

    sf::Vector2i direction(0, 0);

    // Priorité à l'axe horizontal (ou vertical selon ton choix))
    direction.x = (int)m_hAxis;
    if (m_hAxis == 0.f) {
        direction.y = (int)m_vAxis;
    }
    
    m_player.moveRequest(direction, zone);
    m_moveTimer.restart();
}
#include "PlayerController.hpp"
#include "Interactable.hpp"
#include "DialogManager.hpp"
#include <iostream>

PlayerController::PlayerController(World& world, Player& player) : m_world(world), m_player(player) {
    Controller::getInstance().onAxisChanged("MoveHorizontal", [this](float val) {
        m_hAxis = val;
    });
    Controller::getInstance().onAxisChanged("MoveVertical", [this](float val) {
        m_vAxis = val;
    });

    Controller::getInstance().onActionPressed("Interact", [this]() {
        if (DialogManager::getInstance().isActive()) {
            DialogManager::getInstance().next();
        } else {
            sf::Vector2i front = m_player.getFacingTile();
            Zone& currentZone = m_world.getZoneActuelle();
            Interactable* interactable = dynamic_cast<Interactable*>(currentZone.getEntityAt(front.x, front.y));

            if (interactable)
            {
                interactable->interact();
            }
        }
    });
}

void PlayerController::update(Zone& zone, float dt) {
    if (m_moveTimer.getElapsedTime().asSeconds() < m_moveDelay && m_player.getIsMoving()) return;

    sf::Vector2i direction(0, 0);

    // Priorité à l'axe horizontal (ou vertical selon ton choix))
    direction.x = (int)m_hAxis;
    if (m_hAxis == 0.f) {
        direction.y = (int)m_vAxis;
    }
    
    m_player.moveRequest(direction, zone);
    m_moveTimer.restart();
}
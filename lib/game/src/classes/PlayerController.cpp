#include "PlayerController.hpp"

PlayerController::PlayerController(Player& player) : m_player(player) {
    Controller::getInstance().onAxisChanged("MoveHorizontal", [this](float val) {
        m_hAxis = val;
    });
    Controller::getInstance().onAxisChanged("MoveVertical", [this](float val) {
        m_vAxis = val;
    });
}

void PlayerController::update(Zone& zone, float dt) {
    if (m_moveTimer.getElapsedTime().asSeconds() < m_moveDelay) return;

    sf::Vector2i direction(0, 0);

    // Priorité à l'axe horizontal (ou vertical selon ton choix)
    if (m_hAxis != 0.f) {
        direction.x = (m_hAxis > 0.f) ? 1 : -1;
    } else if (m_vAxis != 0.f) {
        direction.y = (m_vAxis > 0.f) ? 1 : -1;
    }

    if (direction.x != 0 || direction.y != 0) {
        m_player.moveRequest(direction, zone);
        m_moveTimer.restart();
    }
}
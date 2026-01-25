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

    // Priorité à l'axe horizontal (ou vertical selon ton choix)) {
    direction.x = (int)m_hAxis;
    if (m_hAxis == 0.f) {
        direction.y = (int)m_vAxis;
    }
    
    m_player.moveRequest(direction, zone);
    m_moveTimer.restart();
}
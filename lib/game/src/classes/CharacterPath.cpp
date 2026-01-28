#include "CharacterPath.hpp"
#include "Character.hpp"
#include "Zone.hpp"
#include <random>
#include <iostream>

CharacterPath::CharacterPath(PathType type, float moveInterval) 
    : m_type(type), 
      m_currentIndex(0), 
      m_forward(true), 
      m_moveInterval(moveInterval) 
{}

void CharacterPath::addDirection(const sf::Vector2i& dir) {
    m_path.push_back(dir);
}

void CharacterPath::update(float dt, Character& character, Zone& zone) {
    if (m_timer.getElapsedTime().asSeconds() < character.getMoveDelay() && character.getIsMoving()) return;

    sf::Vector2i nextMove(0, 0);

    if (m_timer.getElapsedTime().asSeconds() > m_moveInterval) {
        if (m_type == PathType::RANDOM) {
            nextMove = generateRandomDirection();
        } else if (m_type == PathType::SIMPLE && m_currentIndex < m_path.size()) {
            nextMove = m_path[m_currentIndex++];
        } else if (m_type == PathType::LOOP && !m_path.empty()) {
            nextMove = m_path[m_currentIndex];
            updateLoopIndex();
        }
        m_timer.restart();
    }

    character.moveRequest(nextMove, zone);
}

sf::Vector2i CharacterPath::generateRandomDirection() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    int r = dis(gen);
    if (r == 0) return {0, 1};
    if (r == 1) return {0, -1};
    if (r == 2) return {-1, 0};
    return {1, 0};
}

void CharacterPath::updateLoopIndex() {
    if (m_forward) {
        if (m_currentIndex + 1 < m_path.size()) m_currentIndex++;
        else { m_forward = false; for(auto& d : m_path) d = -d; }
    } else {
        if (m_currentIndex > 0) m_currentIndex--;
        else { m_forward = true; for(auto& d : m_path) d = -d; }
    }
}
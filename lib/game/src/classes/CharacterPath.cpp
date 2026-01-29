#include "CharacterPath.hpp"
#include "Character.hpp"
#include "Zone.hpp"
#include <random>
#include <iostream>

CharacterPath::CharacterPath(PathType type, float moveInterval, const sf::Vector2i& startPos, int moveRadius) 
    : m_type(type), 
      m_currentIndex(0), 
      m_forward(true), 
      m_moveInterval(moveInterval),
      m_startpos(startPos),
      m_moveRadius(moveRadius)
{}

void CharacterPath::addDirection(const sf::Vector2i& dir) {
    m_path.push_back(dir);
}

void CharacterPath::addDestination(const sf::Vector2i& currentPos, const sf::Vector2i& dest)
{
    sf::Vector2i delta = dest - currentPos;

    // Horizontal
    int stepX = (delta.x > 0) ? 1 : -1;
    for (int i = 0; i < std::abs(delta.x); ++i)
    {
        m_path.push_back({stepX, 0});
    }

    // Vertical
    int stepY = (delta.y > 0) ? 1 : -1;
    for (int i = 0; i < std::abs(delta.y); ++i)
    {
        m_path.push_back({0, stepY});
    }
}

void CharacterPath::addDestinationLoop(const sf::Vector2i& currentPos, const std::vector<sf::Vector2i>& dest)
{
    sf::Vector2i pos = currentPos;
    for (const auto& d : dest) {
        addDestination(pos, d);
        pos = d;
    }
    addDestination(pos, currentPos);
}

void CharacterPath::update(float dt, Character& character, Zone& zone) {
    if (!m_running) return;
    if (m_character != &character) {
        m_character = &character;
    }
    if (m_timer.getElapsedTime().asSeconds() < character.getMoveDelay() && character.getIsMoving()) return;

    sf::Vector2i nextMove(0, 0);

    if (m_timer.getElapsedTime().asSeconds() > m_moveInterval) {
        if (m_type == PathType::RANDOM) {
            nextMove = generateRandomDirection();
        } else if (m_type == PathType::SIMPLE && m_currentIndex < m_path.size()) {
            nextMove = m_path[m_currentIndex++];
        } else if (m_type == PathType::PINGPONG && !m_path.empty()) {
            nextMove = m_path[m_currentIndex];
            updateLoopIndex();
        } else if (m_type == PathType::LOOP && !m_path.empty()) {
            nextMove = m_path[m_currentIndex];
            m_currentIndex = (m_currentIndex + 1) % m_path.size();
        }
        m_timer.restart();
    }

    character.moveRequest(nextMove, zone);
}

sf::Vector2i CharacterPath::generateRandomDirection() {
    sf::Vector2i pos;
    if (m_character) {
        pos = m_character->getPosition();
    } else {
        pos = m_startpos;
    }
    std::vector<sf::Vector2i> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    std::vector<sf::Vector2i> validDirections;
    for (const auto& d : directions) {
        sf::Vector2i newPos = pos + d;
        if (std::abs(newPos.x - m_startpos.x) <= m_moveRadius && std::abs(newPos.y - m_startpos.y) <= m_moveRadius) {
            validDirections.push_back(d);
        }
    }
    if (validDirections.empty()) return sf::Vector2i(0, 0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validDirections.size() - 1);
    return validDirections[dis(gen)];
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

void CharacterPath::start() {
    m_running = true;
    m_timer.restart();
}

void CharacterPath::pause() {
    m_running = false;
}

bool CharacterPath::isRunning() const {
    return m_running;
}

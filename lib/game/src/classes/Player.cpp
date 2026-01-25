#include "Player.hpp"
#include "ResourceManager.hpp" // AJOUTE CET INCLUDE
#include <iostream>
#include <cmath>

Player::Player(const std::string& name)
    : logicalPos(0,0), orientation(0), targetPos(0.f, 0.f), m_currentAnim("WalkDown")
{
    // Correction de la syntaxe ResourceManager (on précise <sf::Texture>)
    sf::Texture& tex = ResourceManager<sf::Texture>::getInstance().get("./assets/sprite/pnj/player.png");
    m_sprite.setTexture(tex);

    // Initialisation des animations
    m_animations["WalkDown"]  = Animation(0, 4, 0.15f, 64);
    m_animations["WalkLeft"]  = Animation(1, 4, 0.15f, 64);
    m_animations["WalkRight"] = Animation(2, 4, 0.15f, 64);
    m_animations["WalkUp"]    = Animation(3, 4, 0.15f, 64);

    m_name = name;
    
    // On règle l'origine du sprite pour qu'il soit bien centré
    m_sprite.setOrigin(32.f, 48.f); 
    
    setLogicalPos(logicalPos);
}

void Player::moveRequest(sf::Vector2i direction, Zone& zone) {
    if (direction.x == 0 && direction.y == 0) {
        m_ismoving = false;
        m_animations[m_currentAnim].reset();
        return;
    }

    if (direction.y > 0) { orientation = 0; m_currentAnim = "WalkDown"; }
    else if (direction.x < 0) { orientation = 1; m_currentAnim = "WalkLeft"; }
    else if (direction.x > 0) { orientation = 2; m_currentAnim = "WalkRight"; }
    else if (direction.y < 0) { orientation = 3; m_currentAnim = "WalkUp"; }

    m_ismoving = true;

    sf::Vector2i nextPos = logicalPos + direction;
    if (!zone.isBlocking(nextPos.x, nextPos.y)) {
        logicalPos = nextPos;
        targetPos = sf::Vector2f(logicalPos.x * TILE_SIZE + TILE_SIZE/2.f, 
                                 logicalPos.y * TILE_SIZE + TILE_SIZE/2.f);
        GameEvents::OnPlayerMove.notify(logicalPos.x, logicalPos.y);
    } else {
        m_ismoving = false;
        m_animations[m_currentAnim].reset();
    }
}

void Player::update(float dt) {
    if (m_ismoving) {
        m_animations[m_currentAnim].update(dt);
    }
    m_sprite.setTextureRect(m_animations[m_currentAnim].getUVRect());

    // Utilisation de m_sprite partout au lieu de sprite
    sf::Vector2f diff = targetPos - m_sprite.getPosition();
    if (std::abs(diff.x) > 0.5f || std::abs(diff.y) > 0.5f) {
        m_sprite.move(diff * MOVE_SPEED);
    } else {
        m_sprite.setPosition(targetPos);
        m_ismoving = false;
    }
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

sf::Vector2i Player::getPosition() const { return logicalPos; }
sf::Vector2f Player::getDrawPosition() const { return m_sprite.getPosition(); }

sf::Vector2i Player::getFacingTile() const {
    sf::Vector2i front = logicalPos;
    if (orientation == 0) front.y++;
    else if (orientation == 1) front.x--;
    else if (orientation == 2) front.x++;
    else if (orientation == 3) front.y--;
    return front;
}

void Player::setLogicalPos(const sf::Vector2i& pos) {
    logicalPos = pos;
    targetPos = sf::Vector2f(logicalPos.x * TILE_SIZE + TILE_SIZE/2.f, 
                             logicalPos.y * TILE_SIZE + TILE_SIZE/2.f);
    m_sprite.setPosition(targetPos);
}

void Player::receiveItem(const Item& item) {
    m_inventory.addItem(item);
}
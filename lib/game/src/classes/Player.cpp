#include "Player.hpp"
#include <iostream>
#include <cmath>

Player::Player(const std::string& name)
    : logicalPos(0,0), orientation(0), targetPos(0.f, 0.f)
{
    const std::string down  = "./assets/sprite/pnj/player_face.png";
    const std::string left  = "./assets/sprite/pnj/player_l.png";
    const std::string right = "./assets/sprite/pnj/player_r.png";
    const std::string up    = "./assets/sprite/pnj/player_up.png";

    if (!texDown.loadFromFile(down))  std::cerr << "Erreur Down\n";
    if (!texLeft.loadFromFile(left))  std::cerr << "Erreur Left\n";
    if (!texRight.loadFromFile(right)) std::cerr << "Erreur Right\n";
    if (!texUp.loadFromFile(up))     std::cerr << "Erreur Up\n";

    m_name = name;
    sprite.setTexture(texDown);
    sprite.setOrigin(texDown.getSize().x / 2.f, texDown.getSize().y / 2.f);
    
    setLogicalPos(logicalPos);
}

void Player::moveRequest(sf::Vector2i direction, Zone& zone) {
    // 1. Mise à jour de l'orientation (regard)
    if (direction.y > 0) orientation = 0;
    else if (direction.x < 0) orientation = 1;
    else if (direction.x > 0) orientation = 2;
    else if (direction.y < 0) orientation = 3;

    updateTexture();

    // 2. Test de collision
    sf::Vector2i nextPos = logicalPos + direction;
    if (!zone.isBlocking(nextPos.x, nextPos.y)) {
        logicalPos = nextPos;
        targetPos = sf::Vector2f(logicalPos.x * TILE_SIZE + TILE_SIZE/2.f, 
                                 logicalPos.y * TILE_SIZE + TILE_SIZE/2.f - 15.f);
        GameEvents::OnPlayerMove.notify(logicalPos.x, logicalPos.y);
    }
}

void Player::updateTexture() {
    if (orientation == 0) sprite.setTexture(texDown);
    else if (orientation == 1) sprite.setTexture(texLeft);
    else if (orientation == 2) sprite.setTexture(texRight);
    else if (orientation == 3) sprite.setTexture(texUp);
}

void Player::update() {
    sf::Vector2f diff = targetPos - sprite.getPosition();
    if (std::abs(diff.x) > 0.1f || std::abs(diff.y) > 0.1f) {
        sprite.move(diff * MOVE_SPEED);
    } else {
        sprite.setPosition(targetPos);
    }
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::Vector2i Player::getPosition() const { return logicalPos; }
sf::Vector2f Player::getDrawPosition() const { return sprite.getPosition(); }

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
                             logicalPos.y * TILE_SIZE + TILE_SIZE/2.f - 15.f);
    sprite.setPosition(targetPos);
}

void Player::receiveItem(const Item& item) {
    m_inventory.addItem(item);
}
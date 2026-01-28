#include "Character.hpp"
#include "ResourceManager.hpp"
#include "Event.hpp"
#include <cmath>
#include <iostream>

Character::Character(const std::string& name, const std::string& spriteSheetName, const sf::Vector2i& pos, int orientation)
    : WorldEntity(name, pos), 
      m_name(name),
      m_orientation(orientation), 
      m_currentAnim("WalkDown"),
      m_isMoving(false)
{
    // Attention au chemin : assure-toi que le dossier existe
    sf::Texture& tex = ResourceManager<sf::Texture>::getInstance().get(spriteSheetName);
    m_sprite.setTexture(tex);

    m_animations["WalkDown"]  = Animation(0, 4, 0.15f, 64);
    m_animations["WalkLeft"]  = Animation(1, 4, 0.15f, 64);
    m_animations["WalkRight"] = Animation(2, 4, 0.15f, 64);
    m_animations["WalkUp"]    = Animation(3, 4, 0.15f, 64);

    m_sprite.setOrigin(32.f, 32.f); 
    setLogicalPos(m_logicalPos);
}

void Character::stopAnimation() {
    m_isMoving = false;
    m_animations[m_currentAnim].reset();
}

void Character::moveRequest(sf::Vector2i direction, Zone& zone) {
    if (direction.x == 0 && direction.y == 0) {
        m_isMoving = false;
        m_animations[m_currentAnim].reset();
        return;
    }

    // Mise à jour orientation et nom d'animation
    if (direction.y > 0)      { m_orientation = 0; m_currentAnim = "WalkDown"; }
    else if (direction.x < 0) { m_orientation = 1; m_currentAnim = "WalkLeft"; }
    else if (direction.x > 0) { m_orientation = 2; m_currentAnim = "WalkRight"; }
    else if (direction.y < 0) { m_orientation = 3; m_currentAnim = "WalkUp"; }

    m_isMoving = true;

    sf::Vector2i nextPos = m_logicalPos + direction;
    if (!zone.isBlocking(nextPos.x, nextPos.y)) {
        m_logicalPos = nextPos;
        m_targetPos = sf::Vector2f(m_logicalPos.x * TILE_SIZE + TILE_SIZE/2.f, 
                                   m_logicalPos.y * TILE_SIZE);
        
        // Notifier le système que CE personnage a bougé
        //GameEvents::OnCharacterMove.notify(m_logicalPos.x, m_logicalPos.y);
    } else {
        m_isMoving = false;
        m_animations[m_currentAnim].reset();
    }
}

void Character::update(float dt) {
    if (m_isMoving) {
        m_animations[m_currentAnim].update(dt);
    }
    m_sprite.setTextureRect(m_animations[m_currentAnim].getUVRect());

    sf::Vector2f diff = m_targetPos - m_sprite.getPosition();
    if (std::abs(diff.x) > 0.5f || std::abs(diff.y) > 0.5f) {
        m_sprite.move(diff * MOVE_SPEED);
    } else {
        m_sprite.setPosition(m_targetPos);
    }
}

void Character::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

sf::Vector2i Character::getPosition() const { return m_logicalPos; }
sf::Vector2f Character::getDrawPosition() const { return m_sprite.getPosition(); }

sf::Vector2i Character::getFacingTile() const {
    sf::Vector2i front = m_logicalPos;
    if (m_orientation == 0)      front.y++;
    else if (m_orientation == 1) front.x--;
    else if (m_orientation == 2) front.x++;
    else if (m_orientation == 3) front.y--;
    return front;
}

void Character::setLogicalPos(const sf::Vector2i& pos) {
    m_logicalPos = pos;
    m_targetPos = sf::Vector2f(m_logicalPos.x * TILE_SIZE + TILE_SIZE/2.f, 
                               m_logicalPos.y * TILE_SIZE);
    m_sprite.setPosition(m_targetPos);
}

void Character::receiveItem(const Item& item) {
    m_inventory.addItem(item);
}

void Character::RemoveItem(const Item& item, int quantity) {
    m_inventory.removeItem(item, quantity);
}   

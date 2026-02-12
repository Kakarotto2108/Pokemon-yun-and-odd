#include "Character.hpp"
#include "ResourceManager.hpp"
#include "Event.hpp"
#include <cmath>
#include <iostream>

Character::Character(const std::string& name, const std::string& spriteSheetName, const sf::Vector2i& pos, int orientation, std::unique_ptr<CharacterPath> path, const Inventory& inventory)
    : WorldEntity(name, pos), 
      m_name(name),
      m_texturePath(spriteSheetName),
      m_orientation(orientation), 
      m_currentAnim("WalkDown"),
      m_isMoving(false),
      m_inventory(std::make_unique<Inventory>(inventory)),
      m_path(std::move(path))
{
    // Attention au chemin : assure-toi que le dossier existe
    sf::Texture& tex = ResourceManager<sf::Texture>::getInstance().get(spriteSheetName);
    m_sprite.setTexture(tex);
    sf::Vector2u texSize = tex.getSize();
    isAcc = texSize.y / 32 > 2;
    
    if (isAcc) {
        // Les animations doivent correspondre à la taille du sprite (32x32)
        m_animations["WalkUp"]  = Animation(0, 3, 0.15f, 32);
        m_animations["WalkDown"] = Animation(1, 3, 0.15f, 32);
        m_animations["WalkLeft"] = Animation(2, 3, 0.15f, 32);
        m_animations["WalkRight"] = Animation(3, 3, 0.15f, 32);
    } else {
        m_animations["WalkUp"]  = Animation(0, 2, 0.15f, 32);
        m_animations["WalkDown"] = Animation(0, 4, 0.15f, 32, 2);
        m_animations["WalkLeft"] = Animation(1, 3, 0.15f, 32);
        m_animations["WalkRight"] = Animation(1, 3, 0.15f, 32);
    }
    
    m_animations["RunUp"] = Animation(0, 3, 0.2f, 32, 4);
    m_animations["RunDown"] = Animation(1, 3, 0.2f, 32, 4);
    m_animations["RunLeft"] = Animation(2, 3, 0.2f, 32, 4);
    m_animations["RunRight"] = Animation(3, 3, 0.2f, 32, 4);
    m_animations["ReceiveItem1"] = Animation(6,1, 0.2f, 32);
    m_animations["ReceiveItem2"] = Animation(7,1, 0.2f, 32);
    m_animations["ReceiveItem3"] = Animation(8,1, 0.2f, 32);

    m_sprite.setScale(2.f, 2.f);
    setLogicalPos(m_logicalPos);
}

void Character::stopAnimation() {
    m_isMoving = false;
    m_animations[m_currentAnim].reset();
}

void Character::setOrientation(int orientation) {
    m_orientation = orientation;
    std::string prefix = (m_isRunning && m_isMoving) ? "Run" : "Walk";
    switch (m_orientation) {
        case 0: m_currentAnim = prefix + "Down"; m_flipX = false; break;
        case 1: m_currentAnim = prefix + "Left"; m_flipX = false; break;
        case 2: if (isAcc) { m_currentAnim = prefix + "Right"; m_flipX = false; }
                else { m_currentAnim = prefix + "Right"; m_flipX = true; } break;
        case 3: m_currentAnim = prefix + "Up"; m_flipX = false; break;
        default: m_currentAnim = prefix + "Down"; m_flipX = false; break;
    }
}

void Character::startAnimation(const std::vector<std::string>& lstAnim, bool holdLast)
{
    while (!m_animQueue.empty())
        m_animQueue.pop();

    for (const auto& anim : lstAnim)
        m_animQueue.push(anim);

    m_playSequence = true;
    m_holdLastAnim = holdLast;
    m_animTimer = 0.f;

    if (!m_animQueue.empty()) {
        m_currentAnim = m_animQueue.front();
        m_animQueue.pop();
        m_animations[m_currentAnim].reset();
    }
}


void Character::moveRequest(sf::Vector2i direction, Zone& zone) {
    if (direction.x == 0 && direction.y == 0) {
        m_isMoving = false;
        if (!m_playSequence) {
            setOrientation(m_orientation);
            m_animations[m_currentAnim].reset();
        }
        return;
    }

    m_playSequence = false;

    std::string prefix = m_isRunning ? "Run" : "Walk";

    // Mise à jour orientation et nom d'animation
    if (direction.y > 0)      { 
        m_orientation = 0; 
        m_currentAnim = prefix + "Down"; 
        m_flipX = false;
    }
    else if (direction.x < 0) {
        m_orientation = 1;
        m_currentAnim = prefix + "Left";
        m_flipX = false;
    }
    else if (direction.x > 0)
    {
        m_orientation = 2;
        if (!isAcc){
            m_currentAnim = prefix + "Right";
            m_flipX = true;
        }
        else {
            m_currentAnim = prefix + "Right";
            m_flipX = false;
        }
    }
    else if (direction.y < 0) { 
        m_orientation = 3; 
        m_currentAnim = prefix + "Up"; 
        m_flipX = false;
    }

    m_isMoving = true;

    sf::Vector2i nextPos = m_logicalPos + direction;
    if (!zone.isBlocking(nextPos.x, nextPos.y)) {
        m_logicalPos = nextPos;
        m_targetPos = sf::Vector2f(m_logicalPos.x * TILE_SIZE + TILE_SIZE / 2.f, 
                                   (m_logicalPos.y + 1) * TILE_SIZE);
        
        // Notifier le système que CE personnage a bougé
        //GameEvents::OnCharacterMove.notify(m_logicalPos.x, m_logicalPos.y);
    } else {
        m_isMoving = false;
        setOrientation(m_orientation);
        m_animations[m_currentAnim].reset();
    }
}

void Character::update(float dt, Zone& zone) {
    m_sprite.setTextureRect(m_animations[m_currentAnim].getUVRect());

    if (m_flipX) {
        m_sprite.setScale(-2.f, 2.f);
        m_sprite.setOrigin(32.f, 0.f);
    }
    else
    {
        m_sprite.setScale(2.f, 2.f);
        m_sprite.setOrigin(0.f, 0.f);
    }


    if (m_path) {
        m_path->update(dt, *this, zone);
    }

    if (m_isMoving) {
        m_animations[m_currentAnim].update(dt);
    }

    sf::Vector2f diff = m_targetPos - m_sprite.getPosition();
    if (std::abs(diff.x) > 0.5f || std::abs(diff.y) > 0.5f) {
        m_sprite.move(diff * MOVE_SPEED);
    } else {
        m_sprite.setPosition(m_targetPos);
    }
    if (m_playSequence)
    {
        m_animTimer += dt;

        if (m_animTimer >= m_animDelay)
        {
            m_animTimer = 0.f;

            if (!m_animQueue.empty())
            {
                m_currentAnim = m_animQueue.front();
                m_animQueue.pop();
                m_animations[m_currentAnim].reset();
            }
            else
            {
                if (!m_holdLastAnim) {
                    m_playSequence = false;
                }
            }
        }
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
    m_targetPos = sf::Vector2f(m_logicalPos.x * TILE_SIZE + TILE_SIZE / 2.f, 
                               (m_logicalPos.y + 1) * TILE_SIZE);
    m_sprite.setPosition(m_targetPos);
}

void Character::receiveItem(const Item& item) {
    m_inventory->addItem(item, 1);
}

void Character::RemoveItem(const Item& item, int quantity) {
    m_inventory->removeItem(item, quantity);
}   

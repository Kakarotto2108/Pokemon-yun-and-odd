#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <string>

// Définitions de constantes nécessaires pour la compilation (mouvement et taille des tuiles)
const float TILE_SIZE = 32.f;
// NOTE: MOVE_SPEED est maintenant la vitesse réelle de déplacement entre les tuiles.
const float MOVE_SPEED = 0.3f; 

bool eWasPressed = false;

// CORRECTION WREORDER: L'ordre de l'initialisation doit correspondre à l'ordre de déclaration dans Player.hpp.
// En supposant que l'ordre dans Player.hpp est : logicalPos, orientation, textures, sprite.
Player::Player(const std::string& downTexPath,
               const std::string& leftTexPath,
               const std::string& rightTexPath,
               const std::string& upTexPath,
               const std::string& name
                )
    : logicalPos(0,0), 
        sprite(texDown), // 'sprite' est initialisé avec 'texDown'
        orientation(0) // Placé après logicalPos
{
    // Chargement de la texture "Down" (Face)
    if (!texDown.loadFromFile(downTexPath)) {
        // Message de débug critique : vérifiez ce chemin dans votre console !
        std::cerr << "ERREUR FATALE: Impossible de charger la texture 'Down' : " << downTexPath << std::endl;
    }
    // Chargement de la texture "Left"
    if (!texLeft.loadFromFile(leftTexPath)) {
        std::cerr << "ERREUR FATALE: Impossible de charger la texture 'Left' : " << leftTexPath << std::endl;
    }
    // Chargement de la texture "Right"
    if (!texRight.loadFromFile(rightTexPath)) {
        std::cerr << "ERREUR FATALE: Impossible de charger la texture 'Right' : " << rightTexPath << std::endl;
    }
    // Chargement de la texture "Up"
    if (!texUp.loadFromFile(upTexPath)) {
        std::cerr << "ERREUR FATALE: Impossible de charger la texture 'Up' : " << upTexPath << std::endl;
    }

    m_name = name;
    
    // Après le chargement, on s'assure que le sprite utilise la bonne texture (texDown)
    sprite.setTexture(texDown, true); 
    
    // Définition de l'origine au centre du sprite pour une rotation et un positionnement corrects.
    sprite.setOrigin({texDown.getSize().x / 2.f, texDown.getSize().y / 2.f});

    // setScale prend un seul sf::Vector2f, ou une liste d'initialisation {x, y}.
    sprite.setScale({1.f, 1.f}); 

    // Positionnement initial
    // CECI EST CRUCIAL : targetPos est calculé et positionné immédiatement au lieu d'attendre update()
    targetPos = sf::Vector2f(logicalPos.x * TILE_SIZE + TILE_SIZE / 2.f, logicalPos.y * TILE_SIZE + TILE_SIZE / 2.f - 15.f);
    sprite.setPosition(targetPos);
}

int PnjOrientation(sf::Vector2i pnjPos, sf::Vector2i playerPos) {
    if (pnjPos.x == playerPos.x && pnjPos.y == playerPos.y - 1)
        return 0; // down
    else if (pnjPos.x == playerPos.x + 1 && pnjPos.y == playerPos.y)
        return 1; // left
    else if (pnjPos.x == playerPos.x - 1 && pnjPos.y == playerPos.y)
        return 2; // right
    else
        return 3; // up
}

void Player::handleInput(sf::RenderWindow& window, Zone& zone, float delay) {
    int x = logicalPos.x;
    int y = logicalPos.y;
    std::vector<std::string> collisionMap = zone.getCollisionMap();


    // Stocke la position logique actuelle (avant le mouvement)
    sf::Vector2i oldLogicalPos = logicalPos;

    // SFML 3: Les touches sont sf::Keyboard::Key::*
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        orientation = 1;
        sprite.setTexture(texLeft, true); // true = reset rect
        if (x > 0 && collisionMap[y][x - 1] != '*') {
            logicalPos.x--;
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        orientation = 2;
        sprite.setTexture(texRight, true);
        if (x < (int)collisionMap[y].size() - 2 && collisionMap[y][x + 1] != '*') {
            logicalPos.x++;
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        orientation = 3;
        sprite.setTexture(texUp, true);
        if (y > 0 && collisionMap[y - 1][x] != '*') {
            logicalPos.y--;
        }
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        orientation = 0;
        sprite.setTexture(texDown, true);
        if (y + 1 < (int)collisionMap.size() && collisionMap[y + 1][x] != '*') {
            logicalPos.y++;
        }
    }
        
    // Si la position logique a changé, mettez à jour la position cible.
    if (oldLogicalPos != logicalPos) {
        targetPos = sf::Vector2f(logicalPos.x * TILE_SIZE + TILE_SIZE / 2.f, logicalPos.y * TILE_SIZE + TILE_SIZE / 2.f - 15.f);
    }    
}


void Player::update() {
    // Mouvement progressif vers la position cible (interpolation)
    sf::Vector2f diff = targetPos - sprite.getPosition();
    sprite.move(diff * MOVE_SPEED);

}

void Player::draw(sf::RenderWindow& window) const {
    // Dessine le sprite du joueur
    window.draw(sprite);
}

sf::Vector2i Player::getPosition() const {
    return logicalPos;
}

sf::Vector2i Player::getFacingTile() const {
    sf::Vector2i pos = logicalPos;
    switch (orientation) {
        case 0: pos.y += 1; break; // down
        case 1: pos.x -= 1; break; // left
        case 2: pos.x += 1; break; // right
        case 3: pos.y -= 1; break; // up
    }
    return pos;
}

sf::Vector2f Player::getDrawPosition() const {
    return sprite.getPosition();
}

void Player::setLogicalPos(const sf::Vector2i& pos) {
    logicalPos = pos;
    targetPos = sf::Vector2f(logicalPos.x * TILE_SIZE + TILE_SIZE / 2.f,
                             logicalPos.y * TILE_SIZE + TILE_SIZE / 2.f - 15.f);

    sprite.setPosition(targetPos);
}

void Player::receiveItem(const Item& item) {
    // Ajouter l'objet à l'inventaire du joueur
    m_inventory.addItem(item);
    m_inventory.debugPrint();
}
#pragma once
#include <SFML/Graphics.hpp>

class Player;
class MessageBox;

class Interactable {
public:
    virtual ~Interactable() = default;

    virtual sf::Vector2i getPosition() const = 0;

    virtual void interact() = 0;
};

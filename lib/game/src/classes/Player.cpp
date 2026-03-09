#include "Player.hpp"
#include "Item.hpp"
#include <iostream>

Player::Player(const std::string& name) : Character(name, "./assets/sprite/pnj/player.png", sf::Vector2i(0, 0), 0) {}
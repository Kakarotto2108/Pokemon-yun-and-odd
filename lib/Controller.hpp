#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

class Controller {
public:
    Controller();

    void handleInput(sf::RenderWindow& window);

private:
    std::unordered_map<std::string, sf::Keyboard::Key> actionMapping;
    std::unordered_map<std::string, std::pair<sf::Keyboard::Key, sf::Keyboard::Key>> axisMapping;
    std::unordered_set<std::string> pressedActions;
};

#endif // CONTROLLER_HPP
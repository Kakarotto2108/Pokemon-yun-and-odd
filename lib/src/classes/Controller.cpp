#include "Controller.hpp"

// Create action mapping and axis mapping

Controller::Controller() {
    // Example action mapping
    actionMapping["Interact"] = sf::Keyboard::E;

    // Example axis mapping
    axisMapping["MoveHorizontal"] = {sf::Keyboard::Q, sf::Keyboard::D}; // A for left, D for right
    axisMapping["MoveVertical"] = {sf::Keyboard::Z, sf::Keyboard::S};   // W for up, S for down
}

void Controller::handleInput(sf::RenderWindow& window) {
    // Handle axis inputs
    for (const auto& axis : axisMapping) {
        float value = 0.0f;
        if (sf::Keyboard::isKeyPressed(axis.second.first)) {
            value -= 1.0f; // Negative direction
        }
        if (sf::Keyboard::isKeyPressed(axis.second.second)) {
            value += 1.0f; // Positive direction
        }
        // Use the axis value for movement or other purposes
    }

    // Handle action inputs notifies only once when pressed or released
    for (const auto& action : actionMapping) {
        if (sf::Keyboard::isKeyPressed(action.second)) {
            if (pressedActions.find(action.first) == pressedActions.end()) {
                pressedActions.insert(action.first);
                // Action key is pressed
                std::cout << "Action " << action.first << " pressed." << std::endl;
            }
        }
        else {
            if (pressedActions.find(action.first) != pressedActions.end()) {
                pressedActions.erase(action.first);
                // Action key is released
                std::cout << "Action " << action.first << " released." << std::endl;
            }
        }
    }
}
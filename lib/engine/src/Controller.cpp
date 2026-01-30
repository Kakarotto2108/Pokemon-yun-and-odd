#include "Controller.hpp"

Controller::Controller() {
    // Action mapping
    actionMapping["Interact"] = sf::Keyboard::E;
    actionMapping["Save"] = sf::Keyboard::K;
    actionMapping["Load"] = sf::Keyboard::L;
    actionMapping["OpenMenu"] = sf::Keyboard::O;


    // Axis mapping
    axisMapping["MoveHorizontal"] = {sf::Keyboard::Q, sf::Keyboard::D};
    axisMapping["MoveVertical"] = {sf::Keyboard::Z, sf::Keyboard::S};
}

void Controller::onActionPressed(std::string action, ActionCallback callback) {
    pressedCallbacks[action].push_back(callback);
}

void Controller::onActionReleased(std::string action, ActionCallback callback) {
    releasedCallbacks[action].push_back(callback);
}

void Controller::onAxisChanged(std::string axis, AxisCallback callback) {
    axisCallbacks[axis].push_back(callback);
}

void Controller::handleInput(sf::RenderWindow& window) {
    for (const auto& axis : axisMapping) {
        float value = 0.0f;
        if (sf::Keyboard::isKeyPressed(axis.second.first)) value -= 1.0f;
        if (sf::Keyboard::isKeyPressed(axis.second.second)) value += 1.0f;
        
        if (axisCallbacks.count(axis.first)) {
            for (auto& cb : axisCallbacks[axis.first]) {
                cb(value);
            }
        }
    }

    for (const auto& action : actionMapping) {
        bool isDown = sf::Keyboard::isKeyPressed(action.second);
        bool wasDown = (pressedActions.find(action.first) != pressedActions.end());

        if (isDown && !wasDown) {
            pressedActions.insert(action.first);
            if (pressedCallbacks.count(action.first)) {
                for (auto& cb : pressedCallbacks[action.first]) {
                    cb();
                }
            }
        } 
        else if (!isDown && wasDown) {
            pressedActions.erase(action.first);
            if (releasedCallbacks.count(action.first)) {
                for (auto& cb : releasedCallbacks[action.first]) {
                    cb();
                }
            }
        }
    }
}
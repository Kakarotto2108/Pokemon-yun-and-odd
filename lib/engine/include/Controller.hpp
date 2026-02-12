#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <set>

class Controller {
public:
    using ActionCallback = std::function<void()>;
    using AxisCallback = std::function<void(float)>;

    static Controller& getInstance() {
        static Controller instance;
        return instance;
    }

    void onActionPressed(std::string action, ActionCallback callback);
    void onActionReleased(std::string action, ActionCallback callback);
    void onAxisChanged(std::string axis, AxisCallback callback);
    bool isActionActive(const std::string& action) const;

    void handleInput(sf::RenderWindow& window);

private:
    Controller();
    Controller(const Controller&) = delete;
    void operator=(const Controller&) = delete;

    std::map<std::string, sf::Keyboard::Key> actionMapping;
    std::map<std::string, std::pair<sf::Keyboard::Key, sf::Keyboard::Key>> axisMapping;
    std::set<std::string> pressedActions;

    std::map<std::string, std::vector<ActionCallback>> pressedCallbacks;
    std::map<std::string, std::vector<ActionCallback>> releasedCallbacks;
    std::map<std::string, std::vector<AxisCallback>> axisCallbacks;
};

#endif
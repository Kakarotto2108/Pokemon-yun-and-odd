#include "Menu.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include <iostream>

Menu::Menu() {
    Controller::getInstance().onActionPressed("OpenMenu", [this]() {
        if (!GameChoiceBox::getInstance().isVisible()) {
        std::vector<std::pair<std::string, std::string>> choices = {
            {"Pokedex", "OpenPokedex"},
            {"Pokemon", "OpenPokemon"},
            {"Sac", "OpenBag"},
            {Player::getInstance().getName(), "OpenPlayer"},
            {"Sauver", "SaveGame"},
            {"Option", "OpenOptions"}
        };
        GameChoiceBox::getInstance().init(choices);
        GameChoiceBox::getInstance().setChoiceIndex(0);
        GameChoiceBox::getInstance().setVisible(true);
    } else {
        GameChoiceBox::getInstance().setVisible(false);
    }
    });    
}

void Menu::draw(sf::RenderWindow& window) {
    GameChoiceBox::draw(window);
}

Menu::~Menu() {}

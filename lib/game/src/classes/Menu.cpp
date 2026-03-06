#include "Menu.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "DialogManager.hpp"
#include <iostream>

Menu::Menu() {
    Controller::getInstance().onActionPressed("OpenMenu", [this]() {
        if (!this->isVisible() && !DialogManager::getInstance().isActive()) {
            open();
    } else if (m_open) {
        close();
    }
    });    
}

void Menu::open() {
    m_open = true;
    std::vector<std::pair<std::string, std::string>> choices = {
        {"Pokedex", "OpenPokedex"},
        {"Pokemon", "OpenPokemon"},
        {"Sac", "OpenBag"},
        {Player::getInstance().getName(), "OpenPlayer"},
        {"Sauver", "SaveGame"},
        {"Option", "OpenOptions"}
    };
    this->init(choices);
    this->setChoiceIndex(0);
    this->setVisible(true);
    this->setFocus(true);
}

void Menu::close() {
    m_open = false;
    this->setVisible(false);
    this->reset();
}

void Menu::draw(sf::RenderWindow& window) {
    GameChoiceBox::draw(window);
}

Menu::~Menu() {}

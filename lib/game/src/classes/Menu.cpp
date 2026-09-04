#include "Menu.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "DialogManager.hpp"
#include "TeamDisplay.hpp"   // <-- ajouté
#include <iostream>

Menu::Menu() {
    Controller::getInstance().onActionPressed("OpenMenu", [this]() {
        // Priorité : fermer un sous-menu ouvert, où qu'il soit dans sa hiérarchie
        if (TeamDisplay::getInstance().isOpen()) {
            TeamDisplay::getInstance().close();
            this->close();
            return;
        }

        if (!this->isVisible() && !DialogManager::getInstance().isActive()) {
            open();
        } else {
            close();
        }
    });    
}

void Menu::open() {
    m_open = true;
    std::vector<Choice> choices = {
        {"Pokedex", "OpenPokedex", std::monostate()},
        {"Pokemon", "OpenPokemon", std::monostate()},
        {"Sac", "OpenBag", std::monostate()},
        {Player::getInstance().getName(), "OpenPlayer", std::monostate()},
        {"Sauver", "SaveGame", std::monostate()},
        {"Option", "OpenOptions", std::monostate()}
    };
    this->init(choices);
    this->setChoiceIndex(0);
    this->setVisible(true);
    this->setFocus(true);
    this->setPosition({this->getPosition().x, DialogManager::getInstance().getTop()});
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

#ifndef MENU_HPP
#define MENU_HPP

#include "GameChoiceBox.hpp"

class Menu : public GameChoiceBox {
private:

public:
    Menu();
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;
    static Menu& getInstance() {
        static Menu instance;
        return instance;
    }
    ~Menu();
    void draw(sf::RenderWindow& window) override;
};

#endif

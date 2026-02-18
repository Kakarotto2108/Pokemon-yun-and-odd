#ifndef BAG_HPP
#define BAG_HPP
#include "Item.hpp"
#include "GameDialog.hpp"
#include <vector>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

class Bag {
private:
    Bag();
    std::vector<ItemPocket> m_pockets = {ItemPocket::Items,
        ItemPocket::Balls, 
        ItemPocket::KeyItems, 
        ItemPocket::TMsHMs, 
        ItemPocket::Berries};
    int m_currentpocketIndex = 0;
    sf::Clock m_inputClock;
    bool m_isOpen = false;
    GameDialog m_pocketDialog;

    void updateDisplay();
    
public:
    static Bag& getInstance() {
        static Bag instance;
        return instance;
    }
    void open();
    void displayItemDescription();
    void draw(sf::RenderWindow& window);

};

#endif

#ifndef BAG_HPP
#define BAG_HPP
#include "Item.hpp"
#include <vector>
#include <SFML/System/Clock.hpp>

class Bag {
private:
    std::vector<ItemPocket> m_pockets = {ItemPocket::Items,
        ItemPocket::Balls, 
        ItemPocket::KeyItems, 
        ItemPocket::TMsHMs, 
        ItemPocket::Berries};
    int m_currentpocketIndex = 0;
    sf::Clock m_inputClock;

    void updateDisplay();
    
public:
    static Bag& getInstance() {
        static Bag instance;
        return instance;
    }
    void open();

private:
    Bag();
};

#endif

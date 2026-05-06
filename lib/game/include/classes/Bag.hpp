#ifndef BAG_HPP
#define BAG_HPP
#include "GameChoiceBox.hpp"
#include "Item.hpp"
#include "GameDialog.hpp"
#include "ResourceManager.hpp"
#include <vector>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

class Bag {
private:
    Bag();
    std::vector<ItemPocket> m_pockets = {ItemPocket::Items,
        ItemPocket::Heal, 
        ItemPocket::Balls, 
        ItemPocket::KeyItems, 
        ItemPocket::TMsHMs, 
        ItemPocket::Berries};
    int m_currentpocketIndex = 0;
    sf::Clock m_inputClock;
    bool m_isOpen = false;
    GameDialog m_pocketDialog;
    GameChoiceBox m_choiceBox;

    sf::Sprite m_bagSprite;

public:
    static Bag& getInstance() {
        static Bag instance;
        return instance;
    }
    void open();
    void close();
    void displayItemDescription();
    void updateDisplay();
    bool isOpen() const { return m_isOpen; }
    GameChoiceBox& getChoiceBox() { return m_choiceBox; }
    void draw(sf::RenderWindow& window);
    bool lookingForItem = false;
    GameChoiceBox m_subChoiceBox2;
    void resetSubChoiceBox();

};

#endif

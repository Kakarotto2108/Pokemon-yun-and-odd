#ifndef TEAMDISPLAY_HPP
#define TEAMDISPLAY_HPP
#include "Item.hpp"
#include "PokemonInstance.hpp"
#include "GameDialog.hpp"
#include "ResourceManager.hpp"
#include <vector>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

class TeamDisplay {
private:
    TeamDisplay();
    std::vector<ItemPocket> m_pockets = {ItemPocket::Items,
        ItemPocket::Balls, 
        ItemPocket::KeyItems, 
        ItemPocket::TMsHMs, 
        ItemPocket::Berries};
    sf::Clock m_inputClock;
    bool m_isOpen = false;
    GameDialog m_pocketDialog;
    GameDialog m_descriptionDialog;
    int m_currentpocketIndex = 0;
    std::string highlightWithNature(const PokemonInstance& pkm);

    void updateDisplay();
    sf::Sprite m_bagSprite;

public:
    static TeamDisplay& getInstance() {
        static TeamDisplay instance;
        return instance;
    }
    std::vector<PokemonInstance> m_team;
    void open();
    void displayDescription();
    void draw(sf::RenderWindow& window);
    void addPokemon(const PokemonInstance& pkm);
};

#endif

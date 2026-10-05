#ifndef POKEDEX_HPP
#define POKEDEX_HPP
#include "GameChoiceBox.hpp"
#include "Item.hpp"
#include "PokemonInstance.hpp"
#include "GameDialog.hpp"
#include "ResourceManager.hpp"
#include <vector>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>

class Pokedex {
private:
    Pokedex();
    sf::Clock m_inputClock;
    bool m_isOpen = false;
    bool summary = false;
    GameDialog m_descriptionDialog;
    int m_currentpocketIndex = 0;
    std::vector<std::string> m_switchChoice = {};
    sf::Sprite m_bagSprite;

public:
    static Pokedex& getInstance() {
        static Pokedex instance;
        return instance;
    }
    std::vector<PokemonInstance> m_team;
    GameChoiceBox m_choiceBox;
    GameDialog m_pocketDialog;
    GameChoiceBox m_subChoiceBox;
    GameChoiceBox m_moveChoiceBox;
    void open();
    void close();
    bool isOpen() const { return m_isOpen; }
    GameChoiceBox& getChoiceBox() { return m_choiceBox; }
    void displayDescription();
    void updateDisplay();
    GameChoiceBox& getCurrentChoiceBox();
    void resetSubChoiceBox();
    void draw(sf::RenderWindow& window);
    void setSummary(bool value) { summary = value; }
    bool getSummary() const { return summary; }
};

#endif

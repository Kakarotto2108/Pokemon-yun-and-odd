#ifndef POKEDEX_HPP
#define POKEDEX_HPP
#include "GameChoiceBox.hpp"
#include "PokemonInstance.hpp"
#include "GameDialog.hpp"

#include <vector>
#include <SFML/Graphics.hpp>

class Pokedex {
private:
    Pokedex();
    bool m_isOpen = false;
    bool summary = false;
    GameDialog m_descriptionDialog;
    int m_currentpocketIndex = 0;
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
    void open();
    void close();
    bool isOpen() const { return m_isOpen; }
    GameChoiceBox& getChoiceBox() { return m_choiceBox; }
    GameChoiceBox& getSubChoiceBox() { return m_subChoiceBox; }
    void displayDescription();
    void updateDisplay();
    void draw(sf::RenderWindow& window);
    void setSummary(bool value) { summary = value; }
    bool getSummary() const { return summary; }
};

#endif

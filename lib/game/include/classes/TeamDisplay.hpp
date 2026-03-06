#ifndef TEAMDISPLAY_HPP
#define TEAMDISPLAY_HPP
#include "GameChoiceBox.hpp"
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
    sf::Clock m_inputClock;
    bool m_isOpen = false;
    bool summary = false;
    GameDialog m_pocketDialog;
    GameDialog m_descriptionDialog;
    int m_currentpocketIndex = 0;
    std::vector<std::string> m_switchMove = {};
    std::string highlightWithNature(const PokemonInstance& pkm);

    void updateDisplay();
    void updateMove();
    sf::Sprite m_bagSprite;

public:
    static TeamDisplay& getInstance() {
        static TeamDisplay instance;
        return instance;
    }
    std::vector<PokemonInstance> m_team;
    GameChoiceBox m_choiceBox;
    GameChoiceBox m_subChoiceBox;
    GameChoiceBox m_moveChoiceBox;
    void open();
    bool isOpen() const { return m_isOpen; }
    GameChoiceBox& getChoiceBox() { return m_choiceBox; }
    void displayDescription();
    void draw(sf::RenderWindow& window);
    void addPokemon(const PokemonInstance& pkm);
    void addSwitchMove(const std::string& move) { m_switchMove.push_back(move); }
    const std::vector<std::string>& getSwitchMoves() const { return m_switchMove; }
    void switchMove();
    void setSummary(bool value) { summary = value; }
    bool getSummary() const { return summary; }
};

#endif

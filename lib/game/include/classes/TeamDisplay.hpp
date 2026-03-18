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
    GameDialog m_descriptionDialog;
    int m_currentpocketIndex = 0;
    std::vector<std::string> m_switchChoice = {};
    std::string highlightWithNature(const PokemonInstance& pkm);
    void updateMove();
    sf::Sprite m_bagSprite;

public:
    static TeamDisplay& getInstance() {
        static TeamDisplay instance;
        return instance;
    }
    std::vector<PokemonInstance> m_team;
    GameChoiceBox m_choiceBox;
    GameDialog m_pocketDialog;
    GameChoiceBox m_subChoiceBox;
    GameChoiceBox m_moveChoiceBox;
    void open();
    bool isOpen() const { return m_isOpen; }
    GameChoiceBox& getChoiceBox() { return m_choiceBox; }
    void displayDescription();
    void updateDisplay();
    GameChoiceBox& getCurrentChoiceBox();
    void resetSubChoiceBox();
    void draw(sf::RenderWindow& window);
    void addPokemon(const PokemonInstance& pkm);
    void changeEvent(GameChoiceBox choiceBox, std::string eventName);
    void addSwitchChoice(const std::string& choice) { m_switchChoice.push_back(choice); }
    const std::vector<std::string>& getSwitchChoice() const { return m_switchChoice; }
    std::vector<std::string>& getSwitchChoice() { return m_switchChoice; }
    void switchChoice(bool isMove);
    void setSummary(bool value) { summary = value; }
    bool getSummary() const { return summary; }
};

#endif

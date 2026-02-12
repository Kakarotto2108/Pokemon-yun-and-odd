#ifndef GAMECHOICEBOX_HPP
#define GAMECHOICEBOX_HPP
#include "GameDialog.hpp"


class GameChoiceBox : public GameDialog {
private:
    std::vector<std::pair<std::string, std::string>> m_choices;
    int m_currentIndex = 0;
    int m_scrollOffset = 0;
    const int MAX_VISIBLE_CHOICES = 6;
    sf::Sprite m_cursorSprite;
    sf::Sprite m_indicatorSprite;
    sf::Clock m_inputClock; // Horloge pour gérer le cooldown

public:
    GameChoiceBox(const GameChoiceBox&) = delete;
    GameChoiceBox& operator=(const GameChoiceBox&) = delete;

    static GameChoiceBox& getInstance() {
        static GameChoiceBox instance;
        return instance;
    }
    void init(std::vector<std::pair<std::string, std::string>> choices);
    void setChoiceIndex(int index);
    void reset() { m_currentIndex = 0; m_scrollOffset = 0; }
    int getChoiceIndex() const;
    std::string getEventForChoice(const std::string& choiceText);
    void setVisible(bool visible) { m_visible = visible; }
    bool isVisible() const { return m_visible; }
    std::string getChoiceName() const;
    void draw(sf::RenderWindow& window) override;

    GameChoiceBox();
};

#endif

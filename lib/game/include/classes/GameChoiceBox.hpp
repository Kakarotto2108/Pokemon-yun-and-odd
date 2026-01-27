#ifndef GAMECHOICEBOX_HPP
#define GAMECHOICEBOX_HPP
#include "GameDialog.hpp"


class GameChoiceBox : public GameDialog {
private:
    std::map<std::string, std::string> m_choices;
    int m_currentIndex = 0;
    int m_scrollOffset = 0;
    const int MAX_VISIBLE_CHOICES = 6;
    sf::Sprite m_cursorSprite;
    sf::Clock m_inputClock; // Horloge pour gérer le cooldown

    GameChoiceBox();

public:
    GameChoiceBox(const GameChoiceBox&) = delete;
    GameChoiceBox& operator=(const GameChoiceBox&) = delete;

    static GameChoiceBox& getInstance() {
        static GameChoiceBox instance;
        return instance;
    }
    void init(std::map<std::string, std::string> choices);
    void setChoiceIndex(int index);
    int getChoiceIndex() const;
    void setVisible(bool visible) { m_visible = visible; }
    std::string getChoiceName() const;
    void draw(sf::RenderWindow& window) override;
};

#endif

#ifndef GAMECHOICEBOX_HPP
#define GAMECHOICEBOX_HPP
#include "GameDialog.hpp"


class GameChoiceBox : public GameDialog {
private:
    std::vector<std::vector<std::string, std::string>> m_choices;
    bool m_hasFocus = true;
    bool m_hideCursor = false;
    int m_currentIndex = 0;
    int m_scrollOffset = 0;
    int m_visible_choices = 6;
    sf::Sprite m_cursorSprite;
    sf::Sprite m_indicatorSprite;
    sf::Clock m_inputClock; // Horloge pour gérer le cooldown

public:
    GameChoiceBox();

    void init(std::vector<std::vector<std::string, std::string, auto>> choices);
    void open();
    void setChoiceIndex(int index);
    void reset() { m_currentIndex = 0; m_scrollOffset = 0; }
    int getChoiceIndex() const;
    std::string getEventForChoice(const std::string& choiceText);
    void setVisible(bool visible) { m_visible = visible; }
    bool isVisible() const { return m_visible; }
    std::string getChoiceName() const;
    void draw(sf::RenderWindow& window) override;
    sf::Vector2f m_pos = {520.f, 145.f};
    sf::Vector2f m_lastpos;
    void setPosition(const sf::Vector2f& position) { m_pos = position; }
    sf::Vector2f getPosition() const { return m_pos; }
    void setFocus(bool focus) { m_hasFocus = focus; }
    bool hasFocus() const { return m_hasFocus; }
    void hideCursor(bool hide) { m_hideCursor = hide; }
    std::vector<std::vector<std::string, std::string, auto>>& getChoices() { return m_choices; }
    void setMaxVisibleChoices(int max) { m_visible_choices = max; }

    virtual ~GameChoiceBox() = default;
};

#endif

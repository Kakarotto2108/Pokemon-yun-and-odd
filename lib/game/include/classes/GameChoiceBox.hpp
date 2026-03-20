#ifndef GAMECHOICEBOX_HPP
#define GAMECHOICEBOX_HPP
#include "GameDialog.hpp"
#include <variant>

using ChoiceParam = std::variant<std::monostate, int, std::string>;

struct Choice {
    std::string name;
    std::string event;
    ChoiceParam param;

    Choice(const std::string& n, const std::string& e, ChoiceParam p = std::monostate{})
        : name(n), event(e), param(p) {}
};

class GameChoiceBox : public GameDialog {
private:
    std::vector<Choice> m_choices;
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

    void init(std::vector<Choice> choices);
    void open();
    void setChoiceIndex(int index);
    void reset() { m_currentIndex = 0; m_scrollOffset = 0; }
    int getChoiceIndex() const;
    std::string getEventForChoice(const std::string& choiceText);
    void setVisible(bool visible) { m_visible = visible; }
    bool isVisible() const { return m_visible; }
    bool hasParam() const;
    const ChoiceParam& getParamChoice() const;
    std::string getChoiceName() const;
    void draw(sf::RenderWindow& window) override;
    sf::Vector2f m_pos = {520.f, 145.f};
    sf::Vector2f m_lastpos;
    void setPosition(const sf::Vector2f& position) { m_pos = position; }
    sf::Vector2f getPosition() const { return m_pos; }
    void setFocus(bool focus) { m_hasFocus = focus; }
    bool hasFocus() const { return m_hasFocus; }
    void hideCursor(bool hide) { m_hideCursor = hide; }
    std::vector<Choice>& getChoices() { return m_choices; }
    void setMaxVisibleChoices(int max) { m_visible_choices = max; }

    virtual ~GameChoiceBox() = default;
};

#endif

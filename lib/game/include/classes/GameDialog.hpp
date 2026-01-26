#ifndef GAMEDIALOG_HPP
#define GAMEDIALOG_HPP

#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class BoxType { Classic, Object };

struct TextSegment {
    std::string content;
    sf::Color color;
};

class GameDialog {
public:
    GameDialog();

    void setBoxType(BoxType type);
    void setPosition(const sf::Vector2f& pos);
    void setText(const std::string& rawText);
    
    void show() { m_visible = true; }
    void hide() { m_visible = false; }
    bool isVisible() const { return m_visible; }

    void draw(sf::RenderWindow& window);

private:
    void parseText(const std::string& text);
    sf::Color getColorFromName(const std::string& name);

    sf::Sprite m_boxSprite;
    std::vector<TextSegment> m_segments; // Le texte découpé par couleur
    
    bool m_visible = false;
    sf::Vector2f m_pos;
};

#endif
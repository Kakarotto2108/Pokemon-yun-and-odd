#include "MessageBox.hpp"
#include <regex>

MessageBox::MessageBox() {
    setBoxType(BoxType::Classic); 
}

void MessageBox::setBoxType(BoxType type) {
    std::string path = (type == BoxType::Object) 
        ? "assets/sprite/ui/objet_box.png" 
        : "assets/sprite/ui/dialog_box.png";
    
    m_boxSprite.setTexture(TextureManager::getInstance().get(path));
    m_boxSprite.setScale(1.f, 1.f);
}

void MessageBox::setPosition(const sf::Vector2f& pos) {
    m_pos = pos;
    m_boxSprite.setPosition(pos);
}

void MessageBox::parseText(const std::string& text) {
    m_segments.clear();
    sf::Color currentColor = sf::Color::Black;

    // Regex pour capturer $[couleur]
    std::regex colorRegex(R"(\$\[(\w+)\])");
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), colorRegex);
    auto words_end = std::sregex_iterator();

    size_t lastPos = 0;
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        
        if (static_cast<size_t>(match.position()) > lastPos) {
            m_segments.push_back({text.substr(lastPos, match.position() - lastPos), currentColor});
        }
        
        currentColor = getColorFromName(match[1].str());
        lastPos = match.position() + match.length();
    }
    
    if (lastPos < text.length()) {
        m_segments.push_back({text.substr(lastPos), currentColor});
    }
}

sf::Color MessageBox::getColorFromName(const std::string& name) {
    if (name == "blue")  return sf::Color(80, 160, 255);
    if (name == "red")   return sf::Color::Red;
    if (name == "green") return sf::Color::Green;
    if (name == "white") return sf::Color::White;
    return sf::Color::Black; // Couleur par défaut
}

void MessageBox::setText(const std::string& rawText) {
    parseText(rawText);
}

void MessageBox::draw(sf::RenderWindow& window) {
    if (!m_visible) return;

    window.draw(m_boxSprite);

    sf::Font& font = FontManager::getInstance().get("assets/write/write2.otf");

    float currentX = m_pos.x + 40.f;
    float currentY = m_pos.y + 25.f;

    for (const auto& seg : m_segments) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(28);
        t.setFillColor(seg.color);
        t.setString(sf::String::fromUtf8(seg.content.begin(), seg.content.end()));
        t.setPosition(currentX, currentY);
        
        window.draw(t);
        currentX += t.getGlobalBounds().width;
    }
}
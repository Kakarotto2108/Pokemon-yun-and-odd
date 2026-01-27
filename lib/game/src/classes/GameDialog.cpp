#include "GameDialog.hpp"
#include <regex>

GameDialog::GameDialog() {
    setBoxType(BoxType::Classic); 
}

void GameDialog::setBoxType(BoxType type) {
    std::string path = (type == BoxType::Object) 
        ? "assets/sprite/ui/objet_box.png" 
        : "assets/sprite/ui/dialog_box.png";
    
    m_boxSprite.setTexture(TextureManager::getInstance().get(path));
    m_boxSprite.setScale(1.f, 1.f);
}

void GameDialog::setPosition(const sf::Vector2f& pos) {
    m_pos = pos;
    m_boxSprite.setPosition(pos);
}

void GameDialog::parseText(const std::string& text) {
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

sf::Color GameDialog::getColorFromName(const std::string& name) {
    if (name == "blue")  return sf::Color(80, 160, 255);
    if (name == "red")   return sf::Color::Red;
    if (name == "green") return sf::Color::Green;
    if (name == "white") return sf::Color::White;
    if (name == "black") return sf::Color::Black;
    if (name == "yellow")return sf::Color::Yellow;
    if (name == "cyan")  return sf::Color::Cyan;
    if (name == "magenta") return sf::Color::Magenta;
    if (name == "orange") return sf::Color(255, 165, 0);
    if (name == "purple") return sf::Color(128, 0, 128);
    if (name == "pink")  return sf::Color(255, 192, 203);
    if (name == "brown") return sf::Color(165, 42, 42);
    if (name == "gray")  return sf::Color(128, 128, 128);
    if (name == "lightblue") return sf::Color(173, 216, 230);
    if (name == "lightgreen") return sf::Color(144, 238, 144);
    if (name == "lightred") return sf::Color(255, 182, 193);
    if (name == "lightyellow") return sf::Color(255, 255, 224);
    if (name == "lightcyan") return sf::Color(224, 255, 255);
    if (name == "lightmagenta") return sf::Color(255, 182, 255);
    if (name == "gold") return sf::Color(255, 215, 0);
    if (name == "silver") return sf::Color(192, 192, 192);
    if (name == "bronze") return sf::Color(205, 127, 50);
    if (name == "emerald") return sf::Color(0, 255, 0);
    if (name == "ruby") return sf::Color(205, 42, 42);
    if (name == "sapphire") return sf::Color(0, 128, 255);
    return sf::Color::Black; // Couleur par défaut
}

void GameDialog::setText(const std::string& rawText) {
    parseText(rawText);
}

void GameDialog::draw(sf::RenderWindow& window) {
    if (!m_visible) return;

    window.draw(m_boxSprite);

    sf::Font& font = FontManager::getInstance().get("assets/write/write2.otf");

    const unsigned int characterSize = 28;
    float lineSpacing = font.getLineSpacing(characterSize);

    float startX = m_pos.x + 40.f;
    float currentX = startX;
    float currentY = m_pos.y + 25.f;

    for (const auto& seg : m_segments) {
        std::string segmentText = seg.content;
        size_t searchPos = 0;
        
        while(true) {
            size_t newlinePos = segmentText.find('\n', searchPos);
            
            std::string line = segmentText.substr(searchPos, newlinePos - searchPos);

            if(!line.empty()){
                sf::Text t;
                t.setFont(font);
                t.setCharacterSize(characterSize);
                t.setFillColor(seg.color);
                t.setString(sf::String::fromUtf8(line.begin(), line.end()));
                t.setPosition(currentX, currentY);
                
                window.draw(t);
                currentX += t.getGlobalBounds().width;
            }

            if (newlinePos != std::string::npos) {
                currentX = startX;
                currentY += lineSpacing;
                searchPos = newlinePos + 1;
            } else {
                break;
            }
        }
    }
}
#include "MessageBox.hpp"
#include <iostream>


MessageBox::MessageBox(const sf::Vector2f& windowSize, const sf::Vector2f& position)
    : m_dialogSprite(m_dialogTex)
{
    if (!m_font.openFromFile("assets/write/write2.otf")) {
        std::cerr << "Erreur chargement police\n";
    }

    if (!m_dialogTex.loadFromFile("assets/sprite/ui/dialog_box.png")) {
        std::cerr << "Erreur chargement dialog_box.png\n";
    }

    if (!m_dialogObjTex.loadFromFile("assets/sprite/ui/objet_box.png")) {
        std::cerr << "Erreur chargement objet_box.png\n";
    }

    m_dialogSprite.setTexture(m_dialogTex, true);
    m_dialogSprite.setScale({0.8f, 0.8f});

    sf::Vector2f dialogSize(
    m_dialogTex.getSize().x * 0.85f,
    m_dialogTex.getSize().y * 0.5f
    );

    m_whiteFrame.setSize(dialogSize*0.8f);
    m_whiteFrame.setFillColor(sf::Color::Transparent);
    m_whiteFrame.setOutlineColor(sf::Color::White);
    m_whiteFrame.setOutlineThickness(4.f);
}

bool MessageBox::isVisible() const
{
    return m_visible;
}

void MessageBox::CutText(const std::string& text, size_t maxLength) {
    if (text.length() <= maxLength) {
        m_text1 = text;
        m_text2 = "";
        return;
    }
    size_t firstCutPos = maxLength;
    if (text[maxLength] != ' ') {
        size_t lastSpace = text.rfind(' ', maxLength);
        if (lastSpace != std::string::npos) {
            firstCutPos = lastSpace;
        }
    }
    m_text1 = text.substr(0, firstCutPos);
    size_t remainderStart = firstCutPos;
    if (firstCutPos < text.length() && text[firstCutPos] == ' ') {
        remainderStart++;
    }
    std::string remainder = text.substr(remainderStart);
    if (remainder.empty()) {
        m_text2 = "";
    } else if (remainder.length() <= maxLength) {
        m_text2 = remainder;
        m_dialogue = ""; 
    } else {
        size_t secondCutPos = maxLength;
        if (remainder[maxLength] != ' ') {
            size_t lastSpace = remainder.rfind(' ', maxLength);
            if (lastSpace != std::string::npos) {
                secondCutPos = lastSpace;
            }
        }
        m_text2 = remainder.substr(0, secondCutPos);
        m_dialogue = remainder.substr(secondCutPos + 1); 
    }
}

void MessageBox::drawColoredTextLine(
    sf::RenderWindow& window,
    const std::string& line,
    float y,
    const std::string& objectName,
    const sf::Color& normalColor,
    const sf::Color& highlightColor
)
{
    sf::Text left(m_font), obj(m_font), right(m_font), poche(m_font), point(m_font);

    left.setCharacterSize(30);
    obj.setCharacterSize(30);
    right.setCharacterSize(30);
    poche.setCharacterSize(30);
    point.setCharacterSize(30);

    left.setFillColor(normalColor);
    obj.setFillColor(highlightColor);
    right.setFillColor(normalColor);
    poche.setFillColor(highlightColor);
    point.setFillColor(normalColor);

    float x = 40.f;

    size_t objPos = line.find(objectName);

    if (objPos != std::string::npos && !objectName.empty()) {
        std::string leftStr  = line.substr(0, objPos);
        std::string objStr   = objectName;
        std::string rightStr = line.substr(objPos + objectName.size());

        left.setString(sf::String::fromUtf8(leftStr.begin(), leftStr.end()));
        left.setPosition({x, y});
        window.draw(left);
        x += left.getGlobalBounds().size.x;

        obj.setString(sf::String::fromUtf8(objStr.begin(), objStr.end()));
        obj.setPosition({x, y});
        window.draw(obj);
        x += obj.getGlobalBounds().size.x;

        right.setString(sf::String::fromUtf8(rightStr.begin(), rightStr.end()));
        right.setPosition({x, y});
        window.draw(right);
        x += right.getGlobalBounds().size.x;
    }
    else {
        left.setString(sf::String::fromUtf8(line.begin(), line.end()));
        left.setPosition({x, y});
        window.draw(left);
    }
}

void MessageBox::getPocketName(ItemPocket pocket)
{
    switch (pocket) {
        case ItemPocket::Items: m_pocketName = "MÉDICAMENTS"; break;
        case ItemPocket::Balls: m_pocketName = "BALLS"; break;
        case ItemPocket::KeyItems: m_pocketName = "OBJETS RARES"; break;
        case ItemPocket::TMsHMs: m_pocketName = "CT/CS"; break;
        case ItemPocket::Berries: m_pocketName = "BAIES"; break;
    }
}


void MessageBox::draw(sf::RenderWindow& window, std::string& playerName)
{
    if (!m_visible)
        return;

    // --- Position commune ---
    sf::Vector2f dialogPos(
        -52.f,
        window.getSize().y - m_dialogTex.getSize().y + 150.f
    );
    m_dialogSprite.setPosition(dialogPos);
    m_whiteFrame.setPosition(dialogPos + sf::Vector2f(80.f, 5.f));

    // --- Pagination ---
    if (m_canGoNext && !m_dialogue.empty()) {
        CutText(m_dialogue, 35);
        m_canGoNext = false;
    }

    // --- Cas objet ---
    bool showObjectText = m_haveObj && (m_nbrPage <= 2);

    if (showObjectText) {
        m_dialogSprite.setTexture(m_dialogObjTex);
        m_dialogSprite.setScale({0.682f, 0.405f});
        m_dialogSprite.setPosition(dialogPos + sf::Vector2f(79.f, 5.f));
        m_dialogSprite.setColor(sf::Color(255, 255, 255, 128));
    }
    else {
        m_dialogSprite.setTexture(m_dialogTex);
        m_dialogSprite.setColor(sf::Color::White);
    }

    // --- Dessin du fond ---
    if (showObjectText) {
        window.draw(m_whiteFrame);
    }

    window.draw(m_dialogSprite);

    // --- Texte ---
    if (showObjectText) {

        // Construction texte complet
        std::string fullText;

        if (m_nbrPage == 2) {
            fullText = playerName + " reçoit : " + m_objectName + " !";
        } else {
            fullText =
                playerName + " a mis l'objet " + m_objectName + " dans la poche " + m_pocketName + ".";
        }

        // Découpe en 2 lignes max
        std::string line1, line2;

        if (fullText.size() <= 35) {
            line1 = fullText;
        } else {
            size_t cutPos = 35;
            if (fullText[cutPos] != ' ') {
                size_t lastSpace = fullText.rfind(' ', cutPos);
                if (lastSpace != std::string::npos)
                    cutPos = lastSpace;
            }

            line1 = fullText.substr(0, cutPos);

            size_t start = cutPos;
            if (start < fullText.size() && fullText[start] == ' ')
                start++;

            line2 = fullText.substr(start);
        }

        // Dessin lignes colorées
        drawColoredTextLine(
            window, line1, 370.f,
            m_objectName,
            sf::Color::White,
            sf::Color(80, 160, 255)
        );

        if (!line2.empty()) {
            drawColoredTextLine(
                window, line2, 410.f,
                m_pocketName,
                sf::Color::White,
                sf::Color(80, 160, 255)
            );
        }
    }
    else {
        // Texte normal (sans objet)
        sf::Text text(m_font), text2(m_font);

        text.setCharacterSize(30);
        text2.setCharacterSize(30);

        text.setFillColor(sf::Color::Black);
        text2.setFillColor(sf::Color::Black);

        text.setPosition({40.f, 370.f});
        text2.setPosition({40.f, 410.f});

        text.setString(sf::String::fromUtf8(m_text1.begin(), m_text1.end()));
        text2.setString(sf::String::fromUtf8(m_text2.begin(), m_text2.end()));

        window.draw(text);
        window.draw(text2);
    }
}

void MessageBox::setNbrPages(const std::string& text, size_t maxLength)
{
    int pageCount = 0;
    std::string remainingText = text;

    while (!remainingText.empty()) {
        pageCount++;
        size_t cutPos = maxLength;
        if (remainingText.length() <= maxLength) {
            break;
        }
        if (remainingText[maxLength] != ' ') {
            size_t lastSpace = remainingText.rfind(' ', maxLength);
            if (lastSpace != std::string::npos) {
                cutPos = lastSpace;
            }
        }
        size_t nextStart = cutPos;
        if (cutPos < remainingText.length() && remainingText[cutPos] == ' ') {
            nextStart++;
        }
        remainingText = remainingText.substr(nextStart);
    }

    m_nbrPage = (pageCount + 1)/ 2;
    if (m_haveObj) {
        m_nbrPage = m_nbrPage + 2; 
    }
}

int MessageBox::getNbrPages() const
{
    return m_nbrPage;
}

// --- Méthodes de contrôle de l'état ---

void MessageBox::show()
{
    m_visible = true;
    m_canGoNext = true;
}

void MessageBox::hide(sf::RenderWindow& window)
{   
    m_dialogue.clear();
    m_text1.clear();
    m_text2.clear();

    m_haveObj = false;
    m_canGoNext = false;
    m_nbrPage = 0;
    m_visible = false;
    m_dialogue = "";

    // 🔁 RESET VISUEL
    m_dialogSprite.setTexture(m_dialogTex);
    m_dialogSprite.setScale({0.8f, 0.8f});
    m_dialogSprite.setColor(sf::Color::White);

    // Position par défaut
    m_dialogSprite.setPosition({
        -52.f,
        window.getSize().y - m_dialogTex.getSize().y + 150.f}
    );
}

void MessageBox::hasObj()
{
    m_haveObj = true;
}

// --- Méthodes pour le texte et l'objet ---

void MessageBox::setText(const std::string& dialog)
{   
    m_dialogue = dialog;
}

void MessageBox::setObj(const std::string& name)
{   
    m_objectName = name;
}

// --- Méthodes de navigation ---

bool MessageBox::hasNextPage() const
{
    return m_dialogue != "" && m_text2 != ""; 
}

void MessageBox::nextPage()
{
    m_canGoNext = true;
    m_nbrPage--;
}

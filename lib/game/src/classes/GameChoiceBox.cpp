#include "GameChoiceBox.hpp"
#include "Controller.hpp"
#include <iostream>

GameChoiceBox::GameChoiceBox() {
    std::string path = "assets/sprite/ui/cursor.png";
    m_cursorSprite.setTexture(TextureManager::getInstance().get(path));
    m_cursorSprite.setScale(3.f, 3.f);
    m_boxSprite.setTexture(TextureManager::getInstance().get("assets/sprite/ui/dialog_box.png"));
    m_boxSprite.setScale(1.f, 1.f);

    Controller::getInstance().onAxisChanged("MoveVertical", [this](float value) {
        if (!m_visible || m_choices.empty()) return;

        // --- GESTION DU COOLDOWN ---
        // Si moins de 0.2 secondes se sont écoulées, on ignore l'input
        if (m_inputClock.getElapsedTime().asSeconds() < 0.2f) {
            return;
        }

        if (std::abs(value) > 0.1f) { // Zone morte pour éviter les sticks trop sensibles
            if (value < 0) { // Haut
                m_currentIndex--;
                if (m_currentIndex < 0) m_currentIndex = static_cast<int>(m_choices.size()) - 1;
                m_inputClock.restart(); // On relance le cooldown
            } 
            else if (value > 0) { // Bas
                m_currentIndex++;
                if (m_currentIndex >= static_cast<int>(m_choices.size())) m_currentIndex = 0;
                m_inputClock.restart(); // On relance le cooldown
            }
        }
    });
    Controller::getInstance().onActionPressed("Interact", [this]() {
        if (!m_visible) return;
        // Ici on pourrait ajouter une action à exécuter lors de la sélection
        std::cout << "Evenement du choix sélectionné : " << m_choices[getChoiceName()] << std::endl;
        m_visible = false;
    });
}

void GameChoiceBox::init(std::map<std::string, std::string> choices) {
    m_choices = choices;
}

void GameChoiceBox::setChoiceIndex(int index) {
    if (index >= 0 && index < static_cast<int>(m_choices.size())) {
        m_currentIndex = index;
    }
}

int GameChoiceBox::getChoiceIndex() const {
    return m_currentIndex;
}

std::string GameChoiceBox::getChoiceName() const {
    auto it = m_choices.begin();
    std::advance(it, m_currentIndex);
    return it != m_choices.end() ? it->first : "";
}

void GameChoiceBox::draw(sf::RenderWindow& window)
{
    if (!m_visible) return;

    sf::Font& font = FontManager::getInstance().get("assets/write/write2.otf");

    // --- 1. Calculer la hauteur totale nécessaire pour le texte ---
    float totalTextHeight = 0.f;
    const float lineSpacing = 10.f; // espace entre les lignes

    for (const auto& choice : m_choices) {
        setText(choice.first);
        for (const auto& seg : m_segments) {
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(28);
            t.setString(sf::String::fromUtf8(seg.content.begin(), seg.content.end()));
            totalTextHeight += t.getGlobalBounds().height + lineSpacing;
        }
    }

    // Ajouter un padding en haut et en bas
    const float verticalPadding = 10.f;
    totalTextHeight += 3 * verticalPadding;

    // --- 2. Ajuster la taille du sprite ---
    if (m_boxSprite.getTexture()) {
        float originalHeight = static_cast<float>(m_boxSprite.getTexture()->getSize().y);

        float scaleX = 0.2f; // échelle fixe horizontalement
        float scaleY = totalTextHeight / originalHeight; // ajustement vertical automatique
        m_boxSprite.setScale(sf::Vector2f(scaleX, scaleY));
    }

    // On peut ajuster l'origine si nécessaire (ici centrée verticalement et légèrement décalée horizontalement)
    m_boxSprite.setOrigin(-3000.f, 140.f); 
    window.draw(m_boxSprite);

    // --- 3. Positionner le texte à l'intérieur du sprite ---
    sf::FloatRect boxBounds = m_boxSprite.getGlobalBounds();
    float paddingX = 40.f;

    float currentX = boxBounds.left + paddingX;
    float currentY = boxBounds.top + verticalPadding;

    m_cursorSprite.setPosition(currentX - 55.f, currentY + (m_currentIndex - 1) * (lineSpacing*2.65f));
    window.draw(m_cursorSprite);

    for (const auto& choice : m_choices) {
        setText(choice.first);
        for (const auto& seg : m_segments) {
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(28);
            t.setFillColor(seg.color);
            t.setString(sf::String::fromUtf8(seg.content.begin(), seg.content.end()));
            t.setPosition(currentX, currentY);

            currentY += t.getGlobalBounds().height + lineSpacing;
            window.draw(t);
        }
    }
}
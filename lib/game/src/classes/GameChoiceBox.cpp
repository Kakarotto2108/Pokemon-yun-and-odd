#include "GameChoiceBox.hpp"
#include "Controller.hpp"
#include "EventManager.hpp"
#include <iostream>

GameChoiceBox::GameChoiceBox() {
    std::string path = "assets/sprite/ui/cursor.png";
    std::string indicatorPath = "assets/sprite/ui/cursor_indicator.png";
    m_cursorSprite.setTexture(TextureManager::getInstance().get(path));
    m_cursorSprite.setScale(3.f, 3.f);
    m_indicatorSprite.setTexture(TextureManager::getInstance().get(indicatorPath));
    m_indicatorSprite.setScale(3.f, 3.f);
    m_boxSprite.setTexture(TextureManager::getInstance().get("assets/sprite/ui/dialog_box.png"));
    m_boxSprite.setScale(1.f, 1.f);

    Controller::getInstance().onAxisChanged("MoveVertical", [this](float value) {
        if (!m_visible || m_choices.empty()) return;

        // --- GESTION DU COOLDOWN ---
        // Si moins de 0.2 secondes se sont écoulées, on ignore l'input
        if (m_inputClock.getElapsedTime().asSeconds() < 0.15f) {
            return;
        }

        if (std::abs(value) > 0.1f) { // Zone morte pour éviter les sticks trop sensibles
            if (value < 0) { // Haut
                m_currentIndex--;
                if (m_currentIndex < 0) {
                    m_currentIndex = static_cast<int>(m_choices.size()) - 1;
                    m_scrollOffset = std::max(0, static_cast<int>(m_choices.size()) - MAX_VISIBLE_CHOICES);
                } else if (m_currentIndex < m_scrollOffset) {
                    m_scrollOffset = m_currentIndex;
                }
                m_inputClock.restart(); // On relance le cooldown
            } 
            else if (value > 0) { // Bas
                m_currentIndex++;
                if (m_currentIndex >= static_cast<int>(m_choices.size())) {
                    m_currentIndex = 0;
                    m_scrollOffset = 0;
                } else if (m_currentIndex >= m_scrollOffset + MAX_VISIBLE_CHOICES) {
                    m_scrollOffset++;
                }
                m_inputClock.restart(); // On relance le cooldown
            }
        }
    });
    Controller::getInstance().onActionPressed("Interact", [this]() {
        if (!m_visible) return;

        // Sécurité : Si la boîte vient juste de s'ouvrir (moins de 0.2s), on ignore l'appui
        // Cela évite de valider le choix avec la même touche qui a ouvert le menu.
        if (m_inputClock.getElapsedTime().asSeconds() < 0.2f) return;

        std::string eventName = getEventForChoice(getChoiceName());
        
        // On sauvegarde les choix actuels pour détecter s'ils changent (ouverture d'un sous-menu)
        auto previousChoices = m_choices;
        EventManager::getInstance().launchEvent(eventName);
        
        // On ne ferme la boîte que si les choix sont restés les mêmes (action simple)
        if (m_choices == previousChoices) {
            hide();
        }
    });
}

void GameChoiceBox::init(std::vector<std::pair<std::string, std::string>> choices) {
    m_choices = choices;
    m_scrollOffset = 0;
    m_inputClock.restart(); // On reset le timer à l'ouverture pour activer la sécurité
}

void GameChoiceBox::setChoiceIndex(int index) {
    if (index >= 0 && index < static_cast<int>(m_choices.size())) {
        m_currentIndex = index;
    }
}

int GameChoiceBox::getChoiceIndex() const {
    return m_currentIndex;
}

std::string GameChoiceBox::getEventForChoice(const std::string& choiceText) {
    for (const auto& pair : m_choices) {
        if (pair.first == choiceText) {   // pair.first = texte du choix
            return pair.second;           // pair.second = événement associé
        }
    }
    return ""; // ou "NONE", si le choix n'existe pas
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
    float singleLineHeight = 0.f;

    // On calcule la hauteur d'une seule ligne pour être plus robuste
    if (!m_choices.empty()) {
        sf::Text tempText("X", font, 28);
        singleLineHeight = tempText.getGlobalBounds().height + lineSpacing;
        int visibleCount = std::min(static_cast<int>(m_choices.size()), MAX_VISIBLE_CHOICES);
        totalTextHeight = visibleCount * singleLineHeight;
    }

    // Ajouter un padding en haut et en bas
    const float verticalPadding = 10.f;
    totalTextHeight += 3 * verticalPadding; // Padding pour le haut et le bas

    // --- 2. Ajuster la taille du sprite ---
    if (m_boxSprite.getTexture()) {
        float originalWidth = static_cast<float>(m_boxSprite.getTexture()->getSize().x);
        float originalHeight = static_cast<float>(m_boxSprite.getTexture()->getSize().y);

        // Pour que la boîte grandisse vers le haut, on ancre son origine en bas.
        // On ancre aussi à droite pour la positionner facilement sur le côté de l'écran.
        m_boxSprite.setOrigin(originalWidth, originalHeight);

        // On positionne le point d'ancrage (le coin bas-droit) de la boîte.
        // Ici, on la place près du coin bas-droit de la fenêtre, au-dessus du dialogue principal.
        m_boxSprite.setPosition(window.getSize().x - 20.f, window.getSize().y - 145.f);

        float scaleX = 0.3f; // Un peu plus large pour le texte
        float scaleY = totalTextHeight > 0 ? totalTextHeight / originalHeight : 0;
        m_boxSprite.setScale(sf::Vector2f(scaleX, scaleY));
    }

    window.draw(m_boxSprite);

    // --- 3. Positionner le texte à l'intérieur du sprite ---
    sf::FloatRect boxBounds = m_boxSprite.getGlobalBounds();
    float paddingX = 40.f;

    float currentX = boxBounds.left + paddingX;
    float currentY = boxBounds.top + verticalPadding;

    // On positionne le curseur à côté du texte de l'index actuel
    // On ajoute une petite marge (+5.f) pour un meilleur alignement vertical
    int visibleIndex = m_currentIndex - m_scrollOffset;
    m_cursorSprite.setRotation(0.f);
    m_cursorSprite.setOrigin(0.f, 0.f);
    m_cursorSprite.setPosition(currentX - 30.f, currentY + (visibleIndex * singleLineHeight) + 5.f);
    window.draw(m_cursorSprite);

    // On avance l'itérateur jusqu'à l'offset de défilement
    auto it = m_choices.begin();
    for(int i = 0; i < m_scrollOffset; ++i) if(it != m_choices.end()) ++it;

    // On affiche seulement MAX_VISIBLE_CHOICES éléments
    for (int i = 0; i < MAX_VISIBLE_CHOICES && it != m_choices.end(); ++i, ++it) {
        // On dessine directement le texte de chaque choix
        sf::Text t(sf::String::fromUtf8(it->first.begin(), it->first.end()), font, 28);
        t.setFillColor(sf::Color::Black);
        t.setPosition(currentX, currentY);
        
        window.draw(t);
        currentY += singleLineHeight;
    }

    // --- 4. Flèches de défilement ---
    if (m_indicatorSprite.getTexture()) {
        sf::Vector2u cSize = m_indicatorSprite.getTexture()->getSize();
        m_indicatorSprite.setOrigin(cSize.x / 2.f, cSize.y / 2.f);
        float arrowX = boxBounds.left + boxBounds.width - 25.f;

        if (m_scrollOffset > 0) {
            m_indicatorSprite.setRotation(180.f);
            m_indicatorSprite.setPosition(arrowX, boxBounds.top + 25.f);
            window.draw(m_indicatorSprite);
        }

        if (m_scrollOffset + MAX_VISIBLE_CHOICES < static_cast<int>(m_choices.size())) {
            m_indicatorSprite.setRotation(0.f);
            m_indicatorSprite.setPosition(arrowX, boxBounds.top + boxBounds.height - 25.f);
            window.draw(m_indicatorSprite);
        }
    }
}
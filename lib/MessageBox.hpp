#ifndef MESSAGEBOX_HPP
#define MESSAGEBOX_HPP

#include "Item.hpp"
#include "Event.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class MessageBox
{
public:
    // Events
    Event<const Item&> onItemGiven;

    void setPendingItem(const Item& item) {
        m_pendingItem = item; // on stocke l'objet à notifier plus tard
    }

    // Constructeur
    MessageBox(const sf::Vector2f& windowSize, const sf::Vector2f& position);

    // Boucle de jeu
    void draw(sf::RenderWindow& window, std::string& playerName);

    // Visibilité
    bool isVisible() const;
    void show();
    void hide(sf::RenderWindow& window);

    // Texte
    void setText(const std::string& dialog);
    void CutText(const std::string& text, size_t maxLength);

    // Pagination
    bool hasNextPage() const;
    void nextPage(sf::RenderWindow& window, const std::string& dialogue);
    void setNbrPages(const std::string& text, size_t maxLength);
    int  getNbrPages() const;

    // Objet
    void setObj(const std::string& name);
    void getPocketName(ItemPocket pocket);
    void drawText(sf::RenderWindow& window, const std::string& line1, const std::string& line2);

private:
    // --- Ressources ---
    sf::Font    m_font;
    sf::Texture m_dialogTex;
    sf::Texture m_dialogObjTex;
    sf::RectangleShape m_whiteFrame;
    sf::Sprite  m_dialogSprite;

    // --- Texte ---
    std::string m_dialogue;
    std::string m_text1;
    std::string m_text2;

    // --- Objet ---
    std::string m_objectName;
    std::string m_pocketName;
    std::optional<Item> m_pendingItem;
    bool m_haveObj = false;

    // --- État ---
    bool m_visible   = false;
    bool m_canGoNext = false;
    int  m_nbrPage   = 0;

    void drawColoredTextLine(
    sf::RenderWindow& window,
    const std::string& line,
    float y,
    const std::string& objectName,
    const sf::Color& normalColor,
    const sf::Color& highlightColor
    );


};

#endif // MESSAGEBOX_HPP

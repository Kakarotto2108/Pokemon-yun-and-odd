#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Item.hpp"
#include <optional>

class Pnj {
public:
    // Constructeur
    Pnj(const std::string& textureLeftPath,
        const std::string& textureFacePath,
        const sf::Vector2i& position,
        int orientation,
        const std::string& dialogueText,
        std::optional<Item> item = std::nullopt);

    // Méthodes
    void draw(sf::RenderWindow& window) const;
    void update(sf::RenderWindow& window, int orientation);

    bool isInZone(const sf::Vector2f& playerPos) const;
    std::string getDialogue() const;
    sf::Vector2i getPosition() const;
    std::optional<Item> getItem() const;
    void setItemGiven();

    // Public pour accéder directement au sprite
    sf::Sprite m_sprite;
    sf::Vector2i m_position;
    bool m_return;

private:
    // ⚠ Important : textures doivent être déclarées AVANT le sprite
    sf::Texture m_textureLeft;
    sf::Texture m_textureFace;

    int m_orientation;
    std::string m_dialogue;
    std::optional<Item> m_item;
};

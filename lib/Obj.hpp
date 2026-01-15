#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Item.hpp"
#include <optional>

class Obj {
public:
    // Constructeur
    Obj(const std::string& texture,
        const sf::Vector2i& position,
        const std::string& dialogueText,
        std::optional<Item> item = std::nullopt);

    // Méthodes
    void draw(sf::RenderWindow& window) const;

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
    sf::Texture m_texture;

    std::string m_dialogue;
    std::optional<Item> m_item;
};

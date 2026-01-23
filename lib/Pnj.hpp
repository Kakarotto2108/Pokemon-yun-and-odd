#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <optional>
#include "Item.hpp"
#include "Interactable.hpp"
#include "EntiteMonde.hpp"
#include "Event.hpp"

class Pnj : public Interactable, public EntiteMonde {
public:
    // Events
    Event<const Item&> onItemGiven;
    Event<const std::string&> onDialogue;

    // Constructeur
    Pnj(const std::string& textureLeftPath,
        const std::string& textureFacePath,
        const sf::Vector2i& position,
        int orientation,
        const std::string& dialogueText,
        std::optional<Item> item = std::nullopt);

    // Méthodes
    void update(sf::RenderWindow& window, int orientation);
    bool isInZone(const sf::Vector2f& playerPos) const;
    sf::Vector2i getPosition() const override;
    std::string getDialogue() const override;
    void draw(sf::RenderWindow& window) const override;
    void interact() override;
    std::optional<Item> getItem() const;
    std::optional<Item> giveItem() override;

private:
    sf::Sprite m_sprite;
    sf::Vector2i m_position;

    sf::Texture m_textureLeft;
    sf::Texture m_textureFace;
    bool m_return = false;
    int m_orientation;
    std::string m_dialogue;
    std::optional<Item> m_item;
};
 
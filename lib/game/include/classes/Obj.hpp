#pragma once
#include "WorldEntity.hpp"
#include "Interactable.hpp"
#include "Item.hpp"
#include <optional>

// Obj doit hériter de WorldEntity pour être dans le vecteur de la Zone
class Obj : public WorldEntity, public Interactable {
public:
    Obj(const std::string& name, const std::string& texturePath, 
        const sf::Vector2i& pos, const std::string& dialogue, 
        std::optional<Item> item = std::nullopt);
    
    const sf::Sprite& getSprite() const { return m_sprite; }

    // Obligatoire pour Interactable
    void interact() override;

    // Obligatoire pour WorldEntity (Rendu)
    void draw(sf::RenderWindow& window) const override;

private:
    std::string m_dialogue;
    std::optional<Item> m_item;
    sf::Sprite m_sprite;
};
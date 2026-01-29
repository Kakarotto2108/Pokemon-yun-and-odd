#pragma once
#include "WorldEntity.hpp"
#include "Interactable.hpp"
#include "Item.hpp"
#include "EntityState.hpp"
#include <optional>

// Obj doit hériter de WorldEntity pour être dans le vecteur de la Zone
class Obj : public WorldEntity, public Interactable {
public:
    Obj(const std::string& name, const std::string& texturePath, 
        const sf::Vector2i& pos, const std::string& dialogueKey, 
        std::optional<Item> item = std::nullopt);
    
    const sf::Sprite& getSprite() const { return m_sprite; }

    // Obligatoire pour Interactable
    void interact() override;

    // Obligatoire pour WorldEntity (Rendu)
    void draw(sf::RenderWindow& window) const override;

    void applyState(const EntityState& state) override {
        WorldEntity::applyState(state);
        m_dialogueKey = state.dialogKey;
    }

    EntityState getState() const override {
        EntityState state = WorldEntity::getState();
        state.type = EntityType::OBJ;
        state.texturePath = m_texturePath;
        state.orientation = 0; // Les objets n'ont pas d'orientation
        state.dialogKey = m_dialogueKey;
        return state;
    }

private:
    std::string m_dialogueKey;
    std::optional<Item> m_item;
    sf::Sprite m_sprite;
    std::string m_texturePath;
};
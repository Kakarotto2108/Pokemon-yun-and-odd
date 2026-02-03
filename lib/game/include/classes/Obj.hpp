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
        const sf::Vector2f& size, std::optional<Item> item = std::nullopt);
    
    const sf::Sprite& getSprite() const { return m_sprite; }

    void giveItem() { m_item.reset(); }

    // Obligatoire pour Interactable
    void interact() override;

    // Obligatoire pour WorldEntity (Rendu)
    void draw(sf::RenderWindow& window) const override;

    void applyState(const EntityState& state) override {
        WorldEntity::applyState(state);
        m_dialogueKey = state.dialogKey;
        
        if (state.size) {
            m_size = *state.size;
            if (state.type == EntityType::OBJ) {
                m_sprite.setScale(m_size.x, m_size.y * 2.f);
            }
        }
    }

    EntityState getState() const override {
        EntityState state = WorldEntity::getState();
        state.type = EntityType::OBJ;
        state.texturePath = m_texturePath;
        state.orientation = 0; // Les objets n'ont pas d'orientation
        state.dialogKey = m_dialogueKey;
        state.size = m_size;
        return state;
    }

    std::vector<float> getSpriteSize() const {
        sf::Vector2f scale = m_sprite.getScale();
        return {scale.x, scale.y};
    }

    sf::Vector2f getSize() const { return m_size; }

    std::vector<std::pair<float, float>> getAllPositions() const {
        sf::Vector2i pos = getPosition();
        sf::Vector2f size = getSize();
        std::vector<std::pair<float, float>> positions;
        positions.push_back(std::make_pair(m_sprite.getPosition().x, m_sprite.getPosition().y));
        for (int i = 0; i < size.x; i++) {
            for (int j = 0; j < size.y; j++) {
                positions.push_back(std::make_pair(pos.x + i, pos.y - j));
            }
        } 
        return positions;
    }

    void setSize(float sizeX, float sizeY) {
        m_sprite.setScale(sizeX, sizeY);
        m_sprite.setOrigin(sizeX * 8.f, sizeY * 16.f);
    }

    bool getCollision() const { return isColliding; }
    void setCollision(bool collision) { isColliding = collision; }

private:
    std::string m_dialogueKey;
    sf::Vector2f m_size;
    std::optional<Item> m_item;
    sf::Sprite m_sprite;
    std::string m_texturePath;
    bool isColliding = true;

};
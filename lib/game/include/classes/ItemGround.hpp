#pragma once
#include "Interactable.hpp"
#include "WorldEntity.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

class Iog : public WorldEntity, public Interactable {
public:
    Iog(std::string name,sf::Vector2i pos);
    
    void interact() override;
    const sf::Sprite& getSprite() const { return m_sprite; }
    void draw(sf::RenderWindow& window) const override;
    ~Iog() override = default;    

    void applyState(const EntityState& state) override {
        WorldEntity::applyState(state);
    }

    EntityState getState() const override {
        EntityState state = WorldEntity::getState();
        state.type = EntityType::IOG;
        state.orientation = 0; // Les IOG n'ont pas d'orientation
        state.dialogKey = "";
        return state;
    }

private:
    sf::Sprite m_sprite;
    std::string m_name;
};
#pragma once
#include "Character.hpp"
#include "Interactable.hpp"
#include "EntityState.hpp"
#include <iostream>

class Npc : public Character, public Interactable {
public:
    Npc(const std::string& name, const std::string& sprite, sf::Vector2i pos, int orientation, const std::string& dialogueKey, std::unique_ptr<CharacterPath> path = nullptr, const Inventory& inventory = Inventory());

    void interact() override;

    void applyState(const EntityState& state) override {
        Character::applyState(state);
        m_dialogueKey = state.dialogKey;
    }

    EntityState getState() const override {
        EntityState state = Character::getState();
        state.type = EntityType::NPC;
        state.dialogKey = m_dialogueKey;
        return state;
    }

    void setDialogueKey(const std::string& key) {
        m_dialogueKey = key;
    }

private:
    std::string m_dialogueKey;
};
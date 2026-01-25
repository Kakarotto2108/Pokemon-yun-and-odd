#pragma once
#include "Character.hpp"
#include "Interactable.hpp"
#include <iostream>

class Npc : public Character, public Interactable {
public:
    Npc(const std::string& name, const std::string& sprite, sf::Vector2i pos, int orientation, const std::string& dialogue, Item& item);

    void interact() override;

private:
    std::string m_dialogue;
};
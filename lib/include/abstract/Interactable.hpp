#pragma once

#include <optional>
#include "Item.hpp"

class Interactable {
public:
    virtual ~Interactable() = default;

    virtual sf::Vector2i getPosition() const = 0;
    virtual void interact() = 0;

    // --- Nouveaux hooks polymorphes ---
    virtual std::string getDialogue() const { return ""; }

    virtual std::optional<Item> giveItem() { 
        return std::nullopt; 
    }
};

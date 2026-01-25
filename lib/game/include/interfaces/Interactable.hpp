#pragma once

#include <optional>
#include "Item.hpp"
#include "WorldEntity.hpp"

class Interactable {
public:
    virtual ~Interactable() = default;

    virtual void interact() = 0;
};

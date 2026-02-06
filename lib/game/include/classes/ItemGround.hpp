#pragma once
#include "Obj.hpp"

class Iog : public Obj {
public:
    Iog(const std::string& name, sf::Vector2i pos);

    void interact() override;
    EntityState getState() const override;    
    
    std::string toString(ItemPocket pocket)
    {
        switch (pocket)
        {
            case ItemPocket::Items:    return "MÉDICAMENTS";
            case ItemPocket::Balls:    return "BALLS";
            case ItemPocket::KeyItems: return "OBJETS RARES";
            case ItemPocket::TMsHMs:   return "CT&CS";
            case ItemPocket::Berries:  return "BAIES";
            default: return "Unknown";
        }
    }
};

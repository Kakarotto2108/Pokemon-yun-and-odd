#pragma once
#include "Obj.hpp"

class Iog : public Obj {
public:
    Iog(const std::string& name, sf::Vector2i pos);

    void interact() override;
    EntityState getState() const override;
};

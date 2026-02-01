#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"

class Player : public Character {
public:
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    static Player& getInstance() {
        static Player instance("Kakarotto");
        return instance;
    }

    void applyState(const EntityState& state) override {
        Character::applyState(state);
    }

    EntityState getState() const override {
        EntityState state = Character::getState();
        state.type = EntityType::PLAYER;
        return state;
    }

private:
    Player(const std::string& name);
};

#endif
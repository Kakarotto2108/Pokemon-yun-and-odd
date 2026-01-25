#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"

class Player : public Character {
public:
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    static Player& getInstance() {
        static Player instance("Kakrotto");
        return instance;
    }

private:
    Player(const std::string& name);
};

#endif
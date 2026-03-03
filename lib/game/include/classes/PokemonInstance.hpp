#ifndef POKEMONINSTANCE_HPP
#define POKEMONINSTANCE_HPP

#include "Pokemon.hpp"

class PokemonInstance : public Pokemon {
private:
    std::string m_surname;
    int m_level;
    int m_xp;
    std::string m_ability;
    std::vector<std::string> m_currentMoves;
    int xpToLevel(int xp, const std::string& xpType);
public:
    PokemonInstance(const std::string& name);
    void display() const;
};

#endif

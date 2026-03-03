#include "PokemonInstance.hpp"
#include <random>
#include <cmath>

PokemonInstance::PokemonInstance(const std::string& name) : Pokemon() {
    PokemonDataBase& db = PokemonDataBase::getInstance();
    const Pokemon& pkm = db.getPokemon(name);
    m_xp = 125;
    m_surname = name;
    std::random_device rd;              // Seed
    std::mt19937 gen(rd());             // Mersenne Twister
    std::uniform_int_distribution<> dist(0, pkm.m_abilities.size() - 1);

    int randomIndex = dist(gen);
    m_ability = pkm.m_abilities[randomIndex];
    std::vector<std::string> currentMoves;

    m_level = xpToLevel(m_xp, pkm.m_xptype);

    for (const auto& move : pkm.m_moves) {
        if (move.second <= m_level) {
            m_currentMoves.push_back(move.first);
        }
    }
    if (m_currentMoves.size() > 4) {
        m_currentMoves.erase(m_currentMoves.begin(),
                           m_currentMoves.end() - 4);
    }
} 

int PokemonInstance::xpToLevel(int xp, const std::string& xpType) {
    if (xpType == "Fast") {
        return std::cbrt(xp * 5.0 / 4.0);
    }
    else if (xpType == "Mid") {
        printf("Mid XP Type\n");
        return std::cbrt(xp);
    }
    else if (xpType == "Slow") {
        return std::cbrt(xp * 4.0 / 5.0);
    }
    else if (xpType == "Parabol") { // Medium Slow
        int level = 1;
        while (true) {
            int required = (6 * level*level*level)/5 - 15*level*level + 100*level - 140;
            if (required > xp)
                return level - 1;
            level++;
        }
    }
    return 1;
}

void PokemonInstance::display() const {
    printf("Pokemon: %s\n", m_surname.c_str());
    printf("Level: %d\n", m_level);
    printf("XP: %d\n", m_xp);
    printf("Abilities: %s\n", m_ability.c_str());
    printf("\nMoves: ");
    for (const auto& move : m_currentMoves) {
        printf("%s (Level %d) ", move.c_str(), m_level);
    }
    printf("\n");
}


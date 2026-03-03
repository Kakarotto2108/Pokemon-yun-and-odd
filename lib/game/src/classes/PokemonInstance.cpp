#include "PokemonInstance.hpp"
#include <random>
#include <cmath>

PokemonInstance::PokemonInstance(const std::string& name, int level, std::string obj) : Pokemon() {
    PokemonDataBase& db = PokemonDataBase::getInstance();
    const Pokemon& pkm = db.getPokemon(name);
    m_xp = levelToXp(level, pkm.m_xptype);
    m_surname = name;
    if (!pkm.m_abilities.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, pkm.m_abilities.size() - 1);

        int randomIndex = dist(gen);
        m_ability = pkm.m_abilities[randomIndex];
    }
    else {
        std::cerr << "Erreur: Pokemon sans abilities !" << std::endl;
        m_ability = "None";
    }
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
    m_ev = {0, 0, 0, 0, 0, 0};
    m_iv = {rand() % 32, rand() % 32, rand() % 32, rand() % 32, rand() % 32, rand() % 32};
    m_object = obj;
    m_nature = m_natures[rand() % 5][rand() % 5];
} 

int PokemonInstance::xpToLevel(int xp, const std::string& xpType) {
    if (xpType == "Rapide") {
        return std::cbrt(xp * 5.0 / 4.0);
    }
    else if (xpType == "Moyen") {
        return std::cbrt(xp);
    }
    else if (xpType == "Lent") {
        return std::cbrt(xp * 4.0 / 5.0);
    }
    else if (xpType == "Parabole") { 
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

int PokemonInstance::levelToXp(int level, const std::string& xpType) {
    if (xpType == "Rapide") {
        return std::pow(level, 3) * 4.0 / 5.0;
    }
    else if (xpType == "Moyen") {
        return std::pow(level, 3);
    }
    else if (xpType == "Lent") {
        return std::pow(level, 3) * 5.0 / 4.0;
    }
    else if (xpType == "Parabole") {
        return (6 * level*level*level)/5 - 15*level*level + 100*level - 140;
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
    printf("EV: ");
    for (const auto& ev : m_ev) {
        printf("%d ", ev);
    }
    printf("\nIV: ");
    for (const auto& iv : m_iv) {
        printf("%d ", iv);
    }
    printf("\nObject: %s\n", m_object.c_str());
    printf("Nature: %s\n", m_nature.c_str());
    printf("\n");

}


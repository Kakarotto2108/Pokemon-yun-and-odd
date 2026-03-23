#include "PokemonInstance.hpp"
#include <random>
#include <cmath>

PokemonInstance::PokemonInstance(const std::string& name, int level, std::string obj) : Pokemon() {
    PokemonDataBase& db = PokemonDataBase::getInstance();
    const Pokemon& pkm = db.getPokemon(name);
    m_xp = levelToXp(level, pkm.m_xptype);
    m_xpType = pkm.m_xptype;
    m_surname = name;
    std::random_device rd;
    std::mt19937 gen(rd());
    if (!pkm.m_abilities.empty()) {
        
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
    m_encounterLevel = level;

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
    m_item = obj;
    sf::Vector2i posNature = {rand() % 5, rand() % 5};
    m_bonusNature = calculBonusNature(posNature);
    m_nature = m_natures[posNature.x][posNature.y];
    m_description = findDescription(m_iv);
    m_stats = calculStats(pkm.m_base_stats, m_iv, m_ev, m_level, m_bonusNature);
    std::uniform_int_distribution<> dist(0, 65535);
    m_id = dist(gen);
    m_currentPV = m_stats[0] - 10;
    m_sexe = (rand() % 2 == 0) ? "♂" : "♀";
    m_statut = "Brûlé";
}

int PokemonInstance::xpToLevel(int xp, const std::string& xpType) const {
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

int PokemonInstance::levelToXp(int level, const std::string& xpType) const {
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

int PokemonInstance::toNextLevel(int currentLevel, const std::string& xpType) const {
    return levelToXp(currentLevel + 1, xpType) - m_xp;
}

std::string PokemonInstance::findDescription(const std::vector<int>& ivs) const {
    int matchingIndex = -1;
    int max = -1;
    for (size_t i = 0; i < m_iv.size(); ++i) {
        if (m_iv[i] > max) {
            max = m_iv[i];
            matchingIndex = i;
        }
    }
    if (matchingIndex != -1) {
        return m_descriptions[max % 5][matchingIndex];
    }
    return "Description non trouvée.";
}

std::vector<int> PokemonInstance::calculBonusNature(const sf::Vector2i& posNature) const {
    std::vector<int> bonus(5, 0);
    if (posNature.x != posNature.y) {
        bonus[posNature.x] += 1; // Bonus de 10% pour la stat augmentée
        bonus[posNature.y] += -1; // Malus de 10% pour la stat diminuée
    }
    return bonus;
}

std::vector<int> PokemonInstance::calculStats(const std::vector<int>& baseStats, const std::vector<int>& ivs, const std::vector<int>& evs, int level, const std::vector<int>& bonusNature) const {
    std::vector<int> stats;
    stats.push_back(static_cast<int>(floor((baseStats[0] * 2 + ivs[0] + floor(evs[0] / 4)) * level / 100) + level + 10)); // PV
    for (size_t i = 1; i < baseStats.size(); ++i) {
        int stat = static_cast<int>(floor((baseStats[i] * 2 + ivs[i] + floor(evs[i] / 4)) * level / 100 + 5) * (1 + bonusNature[i-1] * 0.1));
        stats.push_back(stat);
    }
    return stats;
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
    printf("\nitem: %s\n", m_item.c_str());
    printf("Nature: %s\n", m_nature.c_str());
    printf("Description: %s\n", m_description.c_str());
    printf("Stats: ");
    for (const auto& stat : m_stats) {
        printf("%d ", stat);
    }
    printf("\nID: %d\n", m_id);
    printf("Current PV: %d\n", m_currentPV);
    printf("Sexe: %s\n", m_sexe.c_str());
    printf("\n\n");

}


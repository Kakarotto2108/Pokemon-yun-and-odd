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
    std::vector<int> m_ev;
    std::vector<int> m_iv;
    std::string m_object;
    std::string m_nature;
    int xpToLevel(int xp, const std::string& xpType);
    int levelToXp(int level, const std::string& xpType);
public:
    PokemonInstance(const std::string& name, int level, std::string obj);
    void display() const;
    std::vector<std::vector<std::string>> m_natures = {
        {"Hardi", "Solo", "Rigide", "Mauvais", "Brave"},
        {"Assuré", "Docile", "Malin", "Lâche", "Relax"},
        {"Modeste", "Doux", "Pudique", "Foufou", "Discret"},
        {"Calme", "Gentil", "Prudent", "Bizarre", "Malpoli"},
        {"Timide", "Pressé", "Jovial", "Naïf", "Serieux"}};
};

#endif

#ifndef POKEMONINSTANCE_HPP
#define POKEMONINSTANCE_HPP

#include "Pokemon.hpp"
#include <SFML/System/Vector2.hpp>

class PokemonInstance : public Pokemon {
private:
    int xpToLevel(int xp, const std::string& xpType) const;
    int levelToXp(int level, const std::string& xpType) const;
    std::vector<int> calculBonusNature(const sf::Vector2i& posNature) const;
    std::vector<int> calculStats(const std::vector<int>& baseStats, const std::vector<int>& ivs, const std::vector<int>& evs, int level, const std::vector<int>& bonusNature) const;
    std::string findDescription(const std::vector<int>& ivs) const;
public:
    PokemonInstance(const std::string& name, int level, std::string obj = "");
    void display() const;
    std::vector<std::vector<std::string>> m_natures = {
        {"Hardi", "Solo", "Rigide", "Mauvais", "Brave"},
        {"Assuré", "Docile", "Malin", "Lâche", "Relax"},
        {"Modeste", "Doux", "Pudique", "Foufou", "Discret"},
        {"Calme", "Gentil", "Prudent", "Bizarre", "Malpoli"},
        {"Timide", "Pressé", "Jovial", "Naïf", "Serieux"}};
    std::vector<std::vector<std::string>> m_descriptions = {
        {"Adore manger.", "Est fier de sa puissance.", "Doté d'un corps robuste.", "Extrêmement curieux.", "Très volontaire.", "Aime courir."},
        {"S'assoupit souvent.", "Aime se démener.", "Sait encaisser les coups.", "Coquin.", "Un peu vaniteux.", "Attentif aux sons."},
        {"Dors beaucoup.", "Un peu coléreux.", "Très obstiné.", "Très astucieux.", "Esprit rebelle.", "Bête et impulsif"},
        {"Éparpille les choses.", "Aime combattre.", "Bonne endurance.", "Souvent dans la lune.", "A horreur de perdre.", "Aime faire le pitre."},
        {"Aime se détendre.", "S'emporte facilement.", "Persévérant.", "Très particulier.", "Assez entêté.", "Fuit rapidement."}
    };
    std::string m_surname;
    int m_level;
    int m_xp;
    std::string m_xpType;
    std::string m_ability;
    std::vector<std::string> m_currentMoves;
    std::vector<int> m_ev;
    std::vector<int> m_iv;
    std::string m_item;
    std::vector<int> m_bonusNature;
    std::string m_nature;
    std::string m_description;
    std::vector<int> m_stats;
    int m_id;
    int m_currentPV;
    std::string m_sexe;
    int m_encounterLevel;
    int toNextLevel(int currentLevel, const std::string& xpType) const;
};

#endif

#include "Pokemon.hpp"

Pokemon::Pokemon(
    int pkdxnumber,
    std::string name,
    std::string type,
    std::string xptype,
    std::vector<std::string> abilities,
    std::string secret_ability,
    int evol_level,
    std::vector<int> base_stats,
    std::string ev_gived,
    int catch_rate,
    std::vector<std::pair<std::string, int>> moves
) : m_pkdxnumber(pkdxnumber),
    m_name(name),
    m_type(type),
    m_xptype(xptype),
    m_abilities(abilities),
    m_secret_ability(secret_ability),
    m_evol_level(evol_level),
    m_base_stats(base_stats),
    m_ev_gived(ev_gived),
    m_catch_rate(catch_rate),
    m_moves(moves) {}


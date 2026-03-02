#include "Pokemon.hpp"

Pokemon::Pokemon(
    int pkdxnumber,
    std::string name,
    std::string type,
    std::string xptype,
    std::vector<std::string> abilities,
    std::vector<int> base_stats,
    std::vector<std::pair<std::string, int>> moves
) : m_pkdxnumber(pkdxnumber),
    m_name(name),
    m_type(type),
    m_xptype(xptype),
    m_abilities(abilities),
    m_base_stats(base_stats),
    m_moves(moves) {}


#ifndef POKEMON_HPP
#define POKEMON_HPP
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

class Pokemon {
public:
    Pokemon() = default;

    Pokemon(
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
    );

    int m_pkdxnumber;
    std::string m_name;
    std::string m_type;
    std::string m_xptype;
    std::vector<std::string> m_abilities;
    std::string m_secret_ability;
    int m_evol_level;
    std::vector<int> m_base_stats;
    std::string m_ev_gived;
    int m_catch_rate;
    std::vector<std::pair<std::string, int>> m_moves;
};

class PokemonDataBase {
public:
    static PokemonDataBase& getInstance() {
        static PokemonDataBase instance;
        return instance;
    }

    std::vector<int> splitToInts(const std::string& str, char delimiter) {
        std::vector<int> result;
        std::istringstream stream(str);
        std::string item;
        while (std::getline(stream, item, delimiter)) {
            try {
                result.push_back(std::stoi(item));
            } catch (const std::invalid_argument&) {
                // Ignore invalid integers
            }
        }
        return result;
    }

    std::vector<std::pair<std::string, int>> splitToMoves(const std::string& str) {
        std::vector<std::pair<std::string, int>> result;
        std::istringstream stream(str);
        std::string item;
        while (std::getline(stream, item, ';')) {
            std::istringstream moveStream(item);
            std::string moveName;
            int moveLevel;
            if (std::getline(moveStream, moveName, ',') && (moveStream >> moveLevel)) {
                result.emplace_back(trim(moveName), moveLevel);
            }
        }
        return result;
    }

    void load(const std::string& filepath){        
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Erreur : Impossible d'ouvrir " << filepath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty() || line[0] == '#') continue;
            // Parse the line and create a Pokemon object
            // For simplicity, let's assume the line is a comma-separated list of values
            auto parts = split(line, '|');
            if (parts.size() >= 4) {
                Pokemon data;
                data.m_pkdxnumber = std::stoi(trim(parts[0]));
                data.m_name = trim(parts[1]);
                data.m_type = trim(parts[2]);
                data.m_xptype = trim(parts[3]);
                data.m_abilities = split(trim(parts[4]), ';');
                data.m_secret_ability = trim(parts[5]);
                data.m_evol_level = std::stoi(trim(parts[6]));
                data.m_base_stats = splitToInts(trim(parts[7]), ';');
                data.m_ev_gived = trim(parts[8]);
                data.m_catch_rate = std::stoi(trim(parts[9]));
                data.m_moves = splitToMoves(trim(parts[10]));

                m_pokemons.emplace(data.m_name, data);
            }
        }
    }

    Pokemon getPokemon(const std::string& name) {
        if (m_pokemons.find(name) != m_pokemons.end()) {
            return m_pokemons.at(name);
        }
        return Pokemon();  // Return a default Pokemon if not found
    }

private:
    std::unordered_map<std::string, Pokemon> m_pokemons;

    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) tokens.push_back(token);
        return tokens;
    }

    std::string trim(const std::string& s) {
        size_t first = s.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = s.find_last_not_of(" \t\n\r");
        return s.substr(first, (last - first + 1));
    }
};
#endif

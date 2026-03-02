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
        std::vector<int> base_stats,
        std::vector<std::pair<std::string, int>> moves
    );

    int m_pkdxnumber;
    std::string m_name;
    std::string m_type;
    std::string m_xptype;
    std::vector<std::string> m_abilities;
    std::vector<int> m_base_stats;
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
                data.m_base_stats = splitToInts(trim(parts[5]), ';');
                data.m_moves = splitToMoves(trim(parts[6]));

                m_pokemons.emplace(data.m_name, data);
                printf("Loaded Pokemon: %s\n", data.m_name.c_str());
                printf("Type: %s\n", data.m_type.c_str());
                printf("XP Type: %s\n", data.m_xptype.c_str());
                printf("Abilities: ");
                for (const auto& ability : data.m_abilities) {
                    printf("%s ", ability.c_str());
                }
                printf("\nBase Stats: ");
                for (const auto& stat : data.m_base_stats) {
                    printf("%d ", stat);
                }
                printf("\nMoves: ");
                for (const auto& move : data.m_moves) {
                    printf("%s (Level %d) ", move.first.c_str(), move.second);
                }
                printf("\n");
            }
        }
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

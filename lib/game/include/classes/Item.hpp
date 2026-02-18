#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Item.hpp"

enum class ItemPocket
{
    Items,
    Balls,
    KeyItems,
    TMsHMs,
    Berries
};

class Item
{
public:

    Item() = default;

    Item(
        std::string name,
        ItemPocket pocket,
        std::string description,
        bool consumable = false
    );

    const std::string& getName() const;
    const std::string& getDescription() const;
    ItemPocket getPocket() const;

    bool isConsumable() const;
    std::string m_name;
    std::string m_description;
    ItemPocket  m_pocket;
    bool        m_consumable;
};

class ItemDatabase {
public:
    static ItemDatabase& getInstance() {
        static ItemDatabase instance;
        return instance;
    }

    void load(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Erreur : Impossible d'ouvrir " << filepath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty() || line[0] == '#') continue;

            auto parts = split(line, '|');
            if (parts.size() >= 4) {
                Item data;
                data.m_name = trim(parts[0]);
                
                std::string pocketStr = trim(parts[1]);
                if (pocketStr == "Balls") data.m_pocket = ItemPocket::Balls;
                else if (pocketStr == "KeyItems") data.m_pocket = ItemPocket::KeyItems;
                else if (pocketStr == "TMsHMs") data.m_pocket = ItemPocket::TMsHMs;
                else if (pocketStr == "Berries") data.m_pocket = ItemPocket::Berries;
                else data.m_pocket = ItemPocket::Items;

                data.m_description = trim(parts[2]);

                std::string consStr = trim(parts[3]);
                data.m_consumable = (consStr == "true" || consStr == "1");

                m_data.emplace(
                    data.m_name,
                    Item(data.m_name, data.m_pocket, data.m_description, data.m_consumable)
                );

            }
        }
        std::cout << "ItemDatabase chargee : " << m_data.size() << " objets." << std::endl;
    }

    Item getItem(const std::string& name) {
        if (m_data.find(name) != m_data.end()) {
            const auto& d = m_data.at(name);
            return Item(d.m_name, d.m_pocket, d.m_description, d.m_consumable);
        }
        return Item(name, ItemPocket::Items, "Description manquante", false);
    }

private:
    std::unordered_map<std::string, Item> m_data;

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
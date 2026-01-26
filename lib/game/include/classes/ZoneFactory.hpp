#ifndef ZONEFACTORY_HPP
#define ZONEFACTORY_HPP

#include "Zone.hpp"
#include "MapLoader.hpp"
#include "ResourceManager.hpp"
#include "ScriptManager.hpp"
#include "Npc.hpp"
#include "Obj.hpp"
#include <iostream>

class ZoneFactory {
public:
    static sf::Vector2i getSpawnPositionForZone(const std::vector<int>& collisionMap,unsigned int width,unsigned int height) 
    {
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                if (collisionMap[x + y * width] == -2) {
                    return {static_cast<int>(x), static_cast<int>(y)};
                }
            }
        }
        return {0, 0};
    }


    static std::map<int, sf::Vector2i> getSpawnPointsForZone(const std::vector<int>& collisionMap,unsigned int width,unsigned int height) 
    {
        std::map<int, sf::Vector2i> spawnPoints;

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                int tileValue = collisionMap[x + y * width];
                if (tileValue < -100) {
                    int index = -tileValue - 100;
                    spawnPoints[index] = {static_cast<int>(x), static_cast<int>(y)};
                }
            }
        }
        return spawnPoints;
    }

    static std::unique_ptr<Zone> createZone(int zoneId) {
        std::string path = "assets/zone/zone" + std::to_string(zoneId) + "/";
        unsigned int width, height;

        // 1. Charger les maps
        auto visual = MapLoader::loadFromFile(path + "map.txt", width, height);
        auto collision = MapLoader::loadFromFile(path + "collisionMap.txt", width, height);
        sf::Texture& tileset = TextureManager::getInstance().get(path + "tileset.png");
        ScriptManager::getInstance().loadDialogues(path + "dialogues.txt");

        // 2. Créer les entités spécifiques à la zone
        std::vector<std::unique_ptr<WorldEntity>> entities;
        std::ifstream entFile(path + "entities.txt");
        std::string line;

        while (std::getline(entFile, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string type, name, sprite, diagKey, xStr, yStr, orienStr;

            // On découpe la ligne : TYPE|NOM|SPRITE|X|Y|ORIEN|DIAG
            std::getline(ss, type, '|');
            std::getline(ss, name, '|');
            std::getline(ss, sprite, '|');
            std::getline(ss, xStr, '|');
            std::getline(ss, yStr, '|');
            std::getline(ss, orienStr, '|');
            std::getline(ss, diagKey, '|');

            sf::Vector2i pos(std::stoi(xStr), std::stoi(yStr));
            int orientation = std::stoi(orienStr);
            std::string fullSpritePath = std::string("assets/sprite/") + (type == "NPC" ? "pnj/" : "obj/") + sprite;

            if (type == "NPC") {
                entities.push_back(std::make_unique<Npc>(name, fullSpritePath, pos, orientation, diagKey));
            } else {
                entities.push_back(std::make_unique<Obj>(name, fullSpritePath, pos, diagKey));
            }
        }

        sf::Vector2i spawnPos = getSpawnPositionForZone(collision, width, height);
        std::map<int, sf::Vector2i> spawnPoints = getSpawnPointsForZone(collision, width, height);

        // 3. Fabriquer la zone
        auto zone = std::make_unique<Zone>(zoneId, width, height, spawnPos, spawnPoints, std::move(collision), std::move(entities), tileset, visual);

        return zone;
    }
};

#endif
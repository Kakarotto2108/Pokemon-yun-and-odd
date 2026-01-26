#ifndef ZONEFACTORY_HPP
#define ZONEFACTORY_HPP

#include "Zone.hpp"
#include "MapLoader.hpp"
#include "ResourceManager.hpp"
#include "Npc.hpp"
#include "Obj.hpp"
#include <iostream>

class ZoneFactory {
public:
    static sf::Vector2i getSpawnPositionForZone(const std::vector<int>& collisionMap, int width) {
        for (size_t i = 0; i < collisionMap.size(); ++i) {
            if (collisionMap[i] == -2) {
                int x = i % width;
                int y = i / width;
                return sf::Vector2i(x, y);
            }
        }
        return sf::Vector2i(0, 0); // Position par défaut si non trouvé
    }

    static std::unique_ptr<Zone> createZone(int zoneId) {
        std::string path = "assets/zone/zone" + std::to_string(zoneId) + "/";
        unsigned int width, height;

        // 1. Charger les maps
        auto visual = MapLoader::loadFromFile(path + "map.txt", width, height);
        auto collision = MapLoader::loadFromFile(path + "collisionMap.txt", width, height);
        sf::Texture& tileset = TextureManager::getInstance().get(path + "tileset.png");

        // 2. Créer les entités spécifiques à la zone
        std::vector<std::unique_ptr<WorldEntity>> entities;

        if (zoneId == 1) {
            // Note : pokeball doit rester en vie assez longtemps ou être copiée. 
            // Si Npc prend une référence (Item&), attention à la portée !
            static Item pokeball("Poké ball", ItemPocket::Balls, "Un objet...", true); 
            
            // Correction NPC : Ajoute le NOM en premier argument si ton constructeur le demande
            entities.push_back(std::make_unique<Npc>(
                "Camille",                         // Nom du PNJ
                "assets/sprite/pnj/gabou.png",    // Sprite
                sf::Vector2i(4, 2),               // Position
                0,                                // Orientation
                "Bonjour !",                      // Dialogue
                pokeball                          // Item
            ));

            // Même logique pour Obj si c'est un Interactable
            entities.push_back(std::make_unique<Obj>(
                "Television",                     // 1. Nom
                "assets/sprite/obj/IMG_1338.png", // 2. Texture Path
                sf::Vector2i(5, 0),               // 3. Position
                "Cette télé est un cadeau de maman.",      // 4. Dialogue
                pokeball                          // 5. Item (optionnel)
            ));
        }
        sf::Vector2i spawnPoint = getSpawnPositionForZone(collision, width);

        // 3. Fabriquer la zone
        auto zone = std::make_unique<Zone>(zoneId, width, height, spawnPoint, std::move(collision), std::move(entities), tileset, visual);

        return zone;
    }
};

#endif
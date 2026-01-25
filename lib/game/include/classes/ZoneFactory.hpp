#ifndef ZONEFACTORY_HPP
#define ZONEFACTORY_HPP

#include "Zone.hpp"
#include "MapLoader.hpp"
#include "ResourceManager.hpp"
#include "Npc.hpp"
#include "Obj.hpp"

class ZoneFactory {
public:
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

        // 3. Fabriquer la zone
        auto zone = std::make_unique<Zone>(zoneId, width, height, std::move(collision), std::move(entities), tileset, visual);

        // 4. Ajouter les transitions (On pourrait aussi les charger d'un fichier)
        if (zoneId == 1) zone->getTransitions().push_back({2, sf::Vector2i(5,1)});
        if (zoneId == 2) zone->getTransitions().push_back({1, sf::Vector2i(5,5)});

        return zone;
    }
};

#endif
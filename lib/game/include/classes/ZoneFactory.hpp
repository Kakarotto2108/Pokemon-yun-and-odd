#ifndef ZONEFACTORY_HPP
#define ZONEFACTORY_HPP

#include "Zone.hpp"
#include "MapLoader.hpp"
#include "ResourceManager.hpp"
#include "Pnj.hpp"
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
        std::vector<std::unique_ptr<Interactable>> entities;

        if (zoneId == 1) {
            Item pokeball("Poké ball", ItemPocket::Balls, "Un objet...", true);
            
            // On ajoute au vecteur d'Interactable (Polymorphisme)
            entities.push_back(std::make_unique<Pnj>(
                "assets/sprite/pnj/48049.png", "assets/sprite/pnj/48050.png",
                sf::Vector2i(4,2), 0, "Bonjour !", pokeball
            ));

            entities.push_back(std::make_unique<Obj>(
                "assets/sprite/obj/IMG_1338.png", sf::Vector2i(5,0),
                "Cette télé est un cadeau.", pokeball
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
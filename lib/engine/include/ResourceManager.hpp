#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>

template <typename Resource>
class ResourceManager {
public:
    // Empêche la copie pour respecter le pattern Singleton (Optionnel mais conseillé)
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Accès simple via une instance statique
    static ResourceManager<Resource>& getInstance() {
        static ResourceManager<Resource> inst;
        return inst;
    }

    // Charger et récupérer une ressource
    Resource& get(const std::string& filepath) {
        auto it = resources.find(filepath);
        
        if (it != resources.end()) {
            return *(it->second);
        }

        // Si non trouvé, on charge
        auto res = std::make_unique<Resource>();
        if (!res->loadFromFile(filepath)) {
            throw std::runtime_error("Erreur : Impossible de charger " + filepath);
        }

        auto& resRef = *res;
        resources[filepath] = std::move(res);
        return resRef;
    }

    // Nettoyer la mémoire si besoin
    void clear() {
        resources.clear();
    }

private:
    ResourceManager() = default;
    std::map<std::string, std::unique_ptr<Resource>> resources;
};

// Aliases pour simplifier l'utilisation maximale
using TextureManager = ResourceManager<sf::Texture>;
using FontManager    = ResourceManager<sf::Font>;
using SoundManager   = ResourceManager<sf::SoundBuffer>;
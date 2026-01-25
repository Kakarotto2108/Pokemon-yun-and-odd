#include "Obj.hpp"
#include "ResourceManager.hpp"
#include <iostream>

Obj::Obj(const std::string& name, const std::string& texturePath, 
         const sf::Vector2i& pos, const std::string& dialogue, 
         std::optional<Item> item)
    : WorldEntity(name, pos), m_dialogue(dialogue), m_item(item) 
{
    // Chargement de la texture via ton Manager
    sf::Texture& tex = TextureManager::getInstance().get(texturePath);
    m_sprite.setTexture(tex);
    
    // Positionnement graphique (TILE_SIZE à adapter selon tes constantes)
    m_sprite.setOrigin(8.f, 16.f);
    m_sprite.setPosition(pos.x * 32.f, pos.y * 32.f); 
    m_sprite.setScale(3.f, 3.f);
}

void Obj::interact() {
    std::cout << m_dialogue << std::endl;
    if (m_item) {
        std::cout << "Vous avez trouvé : " << m_item->getName() << " !" << std::endl;
        m_item = std::nullopt; // On ramasse l'objet
    }
}

void Obj::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}
#include "Obj.hpp"
#include "ResourceManager.hpp"
#include "ScriptManager.hpp"
#include "DialogManager.hpp"
#include <iostream>

Obj::Obj(const std::string& name, const std::string& texturePath, 
         const sf::Vector2i& pos, const std::string& dialogueKey, 
         std::optional<Item> item)
    : WorldEntity(name, pos), m_dialogueKey(dialogueKey), m_item(item) 
{
    m_texturePath = texturePath;
    // Chargement de la texture via ton Manager
    sf::Texture& tex = TextureManager::getInstance().get(m_texturePath);
    m_sprite.setTexture(tex);
    
    // Positionnement graphique (TILE_SIZE à adapter selon tes constantes)
    m_sprite.setOrigin(8.f, 16.f);
    m_sprite.setPosition(pos.x * 32.f, pos.y * 32.f); 
    m_sprite.setScale(3.f, 3.f);

    if (m_texturePath.find("pokeball") != std::string::npos) {
        m_sprite.setScale(1.5f, 1.5f);
    }
}

void Obj::interact() {    
    const auto& script = ScriptManager::getInstance().getDialogue(m_dialogueKey);
    DialogManager::getInstance().startDialogue(script, this);
}

void Obj::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}
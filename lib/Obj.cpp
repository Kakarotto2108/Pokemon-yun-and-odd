#include "Obj.hpp"
#include <iostream>

const float TILE_SIZE_F = 32.f;

// ------ CONSTRUCTEUR ------
Obj::Obj(const std::string& texture,
         const sf::Vector2i& position,
         const std::string& dialogueText,
         std::optional<Item> item)
    : m_position(position),
      m_return(false),
      m_dialogue(dialogueText),
      m_item(item)
{
    // ⚠ Toujours charger les textures avant de les assigner au sprite
    if (!m_texture.loadFromFile(texture))
        std::cerr << "Erreur chargement texture\n";

    // ⚠ Construire le sprite "vide", puis assigner la texture
    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(sf::IntRect(0, 0, m_texture.getSize().x, m_texture.getSize().y));
    m_sprite.setOrigin(m_texture.getSize().x / 2, m_texture.getSize().y / 2);

    // Origine
    const sf::Texture* tex = m_sprite.getTexture(); // SFML 2 compatible
    if (tex)
    {
        sf::Vector2u size = tex->getSize();
        m_sprite.setOrigin(sf::Vector2f(size.x * 0.5f, size.y * 0.5f));
    }

    // Échelle
    m_sprite.setScale(sf::Vector2f(2.f, 2.f));

    // Position
    m_sprite.setPosition(sf::Vector2f(
        position.x * TILE_SIZE_F + TILE_SIZE_F / 2.f,
        position.y * TILE_SIZE_F + TILE_SIZE_F / 2.f
    ));
}

// ------ DIALOGUE / ITEM ------
bool Obj::isInZone(const sf::Vector2f&) const {
    return false;
}

std::string Obj::getDialogue() const {
    return m_dialogue;
}

std::optional<Item> Obj::giveItem() {
    if (m_item.has_value()) {
        Item tmp = *m_item;
        m_item.reset();
        return tmp;
    }
    return std::nullopt;
}


sf::Vector2i Obj::getPosition() const {
    return m_position;
}

void Obj::interact()
{
    if (m_item.has_value()) {
        onItemGiven.notify(*m_item);
        m_item.reset();
    }

    onDialogue.notify(m_dialogue);
}

std::optional<Item> Obj::getItem() const {
    return m_item;
}


void Obj::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}
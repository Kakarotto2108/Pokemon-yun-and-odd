#include "Pnj.hpp"
#include <iostream>

const float TILE_SIZE_F = 32.f;

// ------ CONSTRUCTEUR ------
Pnj::Pnj(const std::string& textureLeftPath,
         const std::string& textureFacePath,
         const sf::Vector2i& position,
         int orientation,
         const std::string& dialogueText,
         std::optional<Item> item)
    : m_position(position),
      m_return(false),
      m_orientation(orientation),
      m_dialogue(dialogueText),
      m_item(item)
{
    // ⚠ Toujours charger les textures avant de les assigner au sprite
    if (!m_textureLeft.loadFromFile(textureLeftPath))
        std::cerr << "Erreur chargement texture gauche\n";

    if (!m_textureFace.loadFromFile(textureFacePath))
        std::cerr << "Erreur chargement texture face\n";

    // ⚠ Construire le sprite "vide", puis assigner la texture
    m_sprite.setTexture(
        (m_orientation == 0 ? m_textureFace : m_textureLeft),
        true
    );

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

// ------ UPDATE ------
void Pnj::update(sf::RenderWindow& window, int orientation) {
    // Exemple : si orientation change ailleurs dans ton code
    if (orientation == 0) {
        m_sprite.setTexture(m_textureFace, true);
        m_sprite.setScale({2.f, 2.f});
        m_return = false;
    }
    else if (orientation == 2) {
        m_sprite.setTexture(m_textureLeft, true);
        m_sprite.setScale({-2.f, 2.f}); // Miroir horizontal
        m_return = true;
    }
    else {
        m_sprite.setTexture(m_textureLeft, true);
        if (m_return) {
            m_sprite.setScale({2.f, 2.f}); // Remet à l'échelle normale
            m_return = false;
        }
    }
}

void Pnj::interact()
{
    if (m_item.has_value()) {
        onItemGiven.notify(*m_item);
        m_item.reset();
    }

    onDialogue.notify(m_dialogue);
}




// ------ DIALOGUE / ITEM ------
bool Pnj::isInZone(const sf::Vector2f&) const {
    return false;
}

std::string Pnj::getDialogue() const {
    return m_dialogue;
}

sf::Vector2i Pnj::getPosition() const {
    return m_position;
}

std::optional<Item> Pnj::getItem() const {
    return m_item;
}

void Pnj::setItemGiven() {
    m_item = std::nullopt;
}

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "ResourceManager.hpp"

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool load(const sf::Texture& texture, sf::Vector2u tileSize, const std::vector<int>& tiles, unsigned int width, unsigned int height) {
        m_tileset = &texture;

        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        for (unsigned int i = 0; i < width; ++i) {
            for (unsigned int j = 0; j < height; ++j) {
                // Récupération du numéro de la tuile
                int tileNumber = tiles[i + j * width];

                // Calcul de la position (tu, tv) dans le tileset
                // On utilise m_tileset qui est maintenant une référence passée au load
                unsigned int columns = m_tileset->getSize().x / tileSize.x;
                int tu = tileNumber % columns;
                int tv = tileNumber / columns;

                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // Positions des 4 coins du Quad
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // Coordonnées de texture correspondantes
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
        }
        return true;
    }

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        states.texture = m_tileset;
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    const sf::Texture* m_tileset;
};
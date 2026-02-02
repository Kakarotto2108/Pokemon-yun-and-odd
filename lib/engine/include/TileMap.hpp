#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif


struct Vertex3D {
    float x, y, z;
    float u, v;
    sf::Color color;
};

class TileMap : public sf::Transformable {
public:
    TileMap() : m_tileset(nullptr) {}

    bool load(const sf::Texture& texture, sf::Vector2u tileSize, const std::vector<int>& tiles, unsigned int width, unsigned int height) {
        m_tileset = &texture;
        m_vertices.clear();
        m_vertices.reserve(width * height * 4);

        float texW = (float)m_tileset->getSize().x;
        float texH = (float)m_tileset->getSize().y;
        unsigned int columns = m_tileset->getSize().x / tileSize.x;

        for (unsigned int j = 0; j < height; ++j) {
            for (unsigned int i = 0; i < width; ++i) {
                int tileNumber = tiles[i + j * width];
                if (tileNumber < 0) continue;

                // Coordonnées en pixels
                float tu = (float)(tileNumber % columns) * tileSize.x;
                float tv = (float)(tileNumber / columns) * tileSize.y;
                float tsX = (float)tileSize.x;
                float tsY = (float)tileSize.y;

                // NORMALISATION pour OpenGL (0.0 à 1.0)
                float u1 = tu / texW;
                float v1 = tv / texH;
                float u2 = (tu + tsX) / texW;
                float v2 = (tv + tsY) / texH;

                float x = (float)i * tsX;
                float y = (float)j * tsY;
                float z = 0.0f;

                sf::Color c = sf::Color::White;

                // On envoie les UV normalisés
                m_vertices.push_back({x,       y,       z, u1, v1, c});
                m_vertices.push_back({x + tsX, y,       z, u2, v1, c});
                m_vertices.push_back({x + tsX, y + tsY, z, u2, v2, c});
                m_vertices.push_back({x,       y + tsY, z, u1, v2, c});
            }
        }
        return true;
    }

    const std::vector<Vertex3D>& getVertices() const { return m_vertices; }
    const sf::Texture* getTileset() const { return m_tileset; }

private:
    std::vector<Vertex3D> m_vertices;
    const sf::Texture* m_tileset;
};
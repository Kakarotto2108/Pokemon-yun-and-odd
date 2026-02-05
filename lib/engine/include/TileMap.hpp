#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#define GL_SILENCE_DEPRECATION
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

        int nbrNeg = 0;
        for (unsigned int i = 0; i < height; i++) {
            for (unsigned int j = 0; j < width; j++) {
                int t = tiles[j + i * width];
                if (t < 0) nbrNeg++;
            }
        }
        

        // Helper pour trouver une tuile à une position visuelle X donnée dans une ligne
        auto getTileAtVisualX = [&](unsigned int row, int targetVisualX) -> int {
            if (row >= height) return -1;
            int currentVX = 0;
            for (unsigned int k = 0; k < width; ++k) {
                int t = tiles[k + row * width];
                if (t < 0) continue; // On saute les -1 (shift)
                
                if (currentVX == targetVisualX) return t;
                currentVX++;
            }
            return -1; // Pas trouvé
        };

        for (unsigned int j = 0; j < height; ++j) {
            int shiftY = 0;
            for (unsigned int jj = 0; jj < j; ++jj) {
                if (tiles[0 + jj * width] < 0)
                    shiftY++;
            }

            for (unsigned int i = 0; i < width; ++i) {
                int rawTile = tiles[i + j * width];
                if (rawTile < 0)  continue; // On décale si -1

                bool isSideWall = rawTile >= 200;
                bool isBackWall = rawTile >= 100 && rawTile < 200;

                int actualTile = rawTile;
                if (isSideWall) actualTile -= 200;
                else if (isBackWall) actualTile -= 100;


                // Coordonnées en pixels
                float tu = (float)(actualTile % columns) * tileSize.x;
                float tv = (float)(actualTile / columns) * tileSize.y;
                float tsX = (float)tileSize.x;
                float tsY = (float)tileSize.y;

                // NORMALISATION pour OpenGL (0.0 à 1.0)
                float u1 = tu / texW;
                float v1 = tv / texH;
                float u2 = (tu + tsX) / texW;
                float v2 = (tv + tsY) / texH;

                // Position X corrigée par le décalage
                int visualX = i - nbrNeg;
                float x = (float)visualX * tsX;
                int visualY = j - shiftY;
                float y = (float)visualY * tsY;

                float z = 0.0f; // Base au sol

                sf::Color c = sf::Color::White;

                if (isSideWall) {
                    // Si la tuile VISUELLEMENT à gauche est aussi un mur latéral -> déjà traité
                    if (visualX > 0) {
                        int left = getTileAtVisualX(j, visualX);
                        if (left >= 200) continue;
                    }

                    // --- MUR LATÉRAL (Orthogonal au fond) ---
                    // On empile sur l'axe X (Gauche -> Droite)
                        
                    // On cherche le "HAUT" du mur (le plus à GAUCHE / Ouest)
                    unsigned int i_start = i;
                    while (i_start > 0) {
                        int t = tiles[(i_start - 1) + j * width]; // Tuile à gauche
                        if (t >= 200) i_start--; // Si c'est un mur latéral, on continue
                        else break;
                    }

                    // On cherche le "BAS" du mur (le plus à DROITE / Est)
                    unsigned int i_end = i;
                    while (i_end + 1 < width) {
                        int t = tiles[(i_end + 1) + j * width]; // Tuile à droite
                        if (t >= 200) i_end++; // Si c'est un mur latéral, on continue
                        else break;
                    }

                    // Coordonnées Z (Hauteur)
                    float z_bottom = -(float)(i_end - i) * tsY;
                    float z_top    = z_bottom - tsY;

                    // On aligne la base X sur la tuile la plus à gauche (i_start)
                    float xBase = (float)(i_start) * tsX;

                    // Inclinaison pour épouser le mur du fond
                    const float slantFactor = 1.0f;
                    float yOffsetBottom = z_bottom * slantFactor;
                    float yOffsetTop = z_top * slantFactor;

                    // Géométrie : Quad dans le plan YZ (face à l'Est)
                    // Y va de j*tsY à (j+1)*tsY (Near -> Far)
                    // Texture : Gauche=Near(y), Droite=Far(y+tsY)
                    m_vertices.push_back({xBase, y + yOffsetBottom,       z_bottom, u1, v2, c}); // Bas-Gauche (Near)
                    m_vertices.push_back({xBase, y + tsY + yOffsetBottom, z_bottom, u2, v2, c}); // Bas-Droite (Far)
                    m_vertices.push_back({xBase, y + tsY + yOffsetTop,    z_top,    u2, v1, c}); // Haut-Droite
                    m_vertices.push_back({xBase, y + yOffsetTop,          z_top,    u1, v1, c}); // Haut-Gauche

                } else if (isBackWall) {
                    // --- MUR DU FOND (Classique) ---
                    // On cherche le HAUT de la colonne (le plus en arrière)

                    // Si la tuile VISUELLEMENT au-dessus est aussi un mur du fond -> déjà traité
                    if (j > 0) {
                        int above = getTileAtVisualX(j - 1, visualX);
                        if (above >= 100 && above < 200) continue;
                    }

                    unsigned int j_start = j;
                    while (j_start > 0 && tiles[i + (j_start - 1) * width] >= 0 && tiles[i + (j_start - 1) * width] >= 100) {
                        j_start--;
                    }

                    // On cherche le BAS de la colonne (le plus en avant)
                    unsigned int j_end = j;
                    while (j_end + 1 < height) {
                        int nextTile = tiles[i + (j_end + 1) * width];
                        if (nextTile < 100 || (nextTile >= 200)) break; // Ce n'est plus un mur du fond
                        j_end++;
                    }

                    const float slantFactor = 1.0f; 
                    float wallPixelHeight = (float)(j_end - j_start + 1) * tsY;
                    float yOffset = wallPixelHeight * slantFactor;

                    float z_bottom = -(float)(j_end - j) * tsY;
                    float z_top    = z_bottom - tsY;

                    float y_bottom = (float)j_start * tsY + (z_bottom / wallPixelHeight) * yOffset;
                    float y_top    = (float)j_start * tsY + (z_top / wallPixelHeight) * yOffset;

                    m_vertices.push_back({x,       y_bottom, z_bottom, u1, v2, c}); 
                    m_vertices.push_back({x + tsX, y_bottom, z_bottom, u2, v2, c}); 
                    m_vertices.push_back({x + tsX, y_top,    z_top,    u2, v1, c}); 
                    m_vertices.push_back({x,       y_top,    z_top,    u1, v1, c}); 
                    
                } else {
                    // Sol plat classique
                    m_vertices.push_back({x,       y,       z, u1, v1, c});
                    m_vertices.push_back({x + tsX, y,       z, u2, v1, c});
                    m_vertices.push_back({x + tsX, y + tsY, z, u2, v2, c});
                    m_vertices.push_back({x,       y + tsY, z, u1, v2, c});
                }
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
#include "TileMap.hpp"
#include <iostream>
// Rétablissement de l'inclusion car sf::PrimitiveType est nécessaire.
#include <SFML/Graphics/PrimitiveType.hpp> 

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize,
                   const std::vector<std::string>& map,
                   unsigned int width, unsigned int height) {
    // CORRECTION 1: openFromFile remplacé par loadFromFile (SFML 3.0.2)
    if (!m_tileset.loadFromFile(tileset))
        return false;

    // CORRECTION 2: Utilisation de sf::PrimitiveType::Triangles (6 sommets par tuile) pour remplacer sf::Quads dans SFML 3.0.2.
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles); 
    // Chaque tuile est composée de 2 triangles, soit 6 sommets.
    m_vertices.resize(width * height * 6);

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            
            if (i >= map.size() || j >= map[i].size()) {
                std::cerr << "Avertissement: Indice de carte TileMap hors limites. Ignorer.\n";
                continue;
            }

            int tileNumber = map[i][j] - '0';
            
            if (tileNumber < 0 || tileNumber > 9) {
                 continue;
            }
            

            int tilesetColumns = m_tileset.getSize().x / tileSize.x;
            int tu = tileNumber % tilesetColumns;
            int tv = tileNumber / tilesetColumns;

            // Pointeur vers le premier des 6 sommets du triangle-paire
            sf::Vertex* triangles = &m_vertices[(i * width + j) * 6];

            // Définition des coordonnées de la tuile
            float x = j * tileSize.x;
            float y = i * tileSize.y;

            // Définition des coordonnées de texture
            float tx = tu * tileSize.x;
            float ty = tv * tileSize.y;

            // Triangle 1 (Haut-Gauche, Bas-Gauche, Haut-Droit)
            // 0: Haut-Gauche
            triangles[0].position = sf::Vector2f(x, y);
            triangles[0].texCoords = sf::Vector2f(tx, ty);

            // 1: Bas-Gauche
            triangles[1].position = sf::Vector2f(x, y + tileSize.y);
            triangles[1].texCoords = sf::Vector2f(tx, ty + tileSize.y);
            
            // 2: Haut-Droit
            triangles[2].position = sf::Vector2f(x + tileSize.x, y);
            triangles[2].texCoords = sf::Vector2f(tx + tileSize.x, ty);

            // Triangle 2 (Haut-Droit, Bas-Gauche, Bas-Droit)
            // 3: Haut-Droit (dupliqué du 2)
            triangles[3].position = sf::Vector2f(x + tileSize.x, y);
            triangles[3].texCoords = sf::Vector2f(tx + tileSize.x, ty);

            // 4: Bas-Gauche (dupliqué du 1)
            triangles[4].position = sf::Vector2f(x, y + tileSize.y);
            triangles[4].texCoords = sf::Vector2f(tx, ty + tileSize.y);

            // 5: Bas-Droit
            triangles[5].position = sf::Vector2f(x + tileSize.x, y + tileSize.y);
            triangles[5].texCoords = sf::Vector2f(tx + tileSize.x, ty + tileSize.y);
        }
    }

    return true;
}

// CORRECTION: Implémentation de la méthode draw, qui était déclarée mais pas définie.
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Appliquer la transformation
    states.transform *= getTransform();

    // Appliquer la texture du tileset
    states.texture = &m_tileset;

    // Dessiner le tableau de sommets
    target.draw(m_vertices, states);
}
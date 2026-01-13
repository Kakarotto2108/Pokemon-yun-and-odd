#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool load(const std::string& tileset, sf::Vector2u tileSize,
              const std::vector<std::string>& map,
              unsigned int width, unsigned int height);

private:
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // TILEMAP_HPP

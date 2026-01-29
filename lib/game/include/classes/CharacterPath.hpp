#ifndef CHARACTER_PATH_HPP
#define CHARACTER_PATH_HPP

#include <vector>
#include <SFML/System.hpp>

// Forward declarations
class Character;
class Zone;

enum class PathType { RANDOM, SIMPLE, LOOP, PINGPONG };

class CharacterPath {
public:
    CharacterPath(PathType type, float moveInterval = 1.0f);

    void addDirection(const sf::Vector2i& dir);
    void addDestination(const sf::Vector2i& currentPos, const sf::Vector2i& dest);
    void addDestinationLoop(const sf::Vector2i& currentPos, const std::vector<sf::Vector2i>& dest);
    void update(float dt, Character& character, Zone& zone);

    void start();
    void pause();
    bool isRunning() const;

private:
    sf::Vector2i generateRandomDirection();
    void updateLoopIndex();

    // Respecter l'ordre de déclaration pour éviter les warnings [-Wreorder]
    PathType m_type;
    std::vector<sf::Vector2i> m_path;
    size_t m_currentIndex;
    bool m_forward;
    float m_moveInterval;
    sf::Clock m_timer;
    bool m_running = true;
};

#endif
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
public:
    Animation() = default;
    Animation(int line, int frameCount, float switchTime, int spriteSize);

    void update(float dt);
    const sf::IntRect& getUVRect() const { return m_uvRect; }
    void reset();

private:
    private:
    int m_line;         
    int m_frameCount;   
    float m_switchTime; 
    int m_spriteSize;   
    int m_currentFrame;
    int m_targetFrame = 1;
    bool m_backToZero = false;
    float m_totalTime;  
    sf::IntRect m_uvRect;
};
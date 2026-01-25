#include "Animation.hpp"

Animation::Animation(int line, int frameCount, float switchTime, int spriteSize)
    : m_line(line)
    , m_frameCount(frameCount)
    , m_switchTime(switchTime)
    , m_spriteSize(spriteSize)
    , m_currentFrame(0)
    , m_totalTime(0.f)
{
    // Initialisation du premier rectangle
    m_uvRect.width = m_spriteSize;
    m_uvRect.height = m_spriteSize;
    m_uvRect.top = m_line * m_spriteSize;
    m_uvRect.left = 0;
}

void Animation::update(float dt) {
    m_totalTime += dt;

    if (m_totalTime >= m_switchTime) {
        m_totalTime -= m_switchTime;
        m_currentFrame++;

        if (m_currentFrame >= m_frameCount) {
            m_currentFrame = 0;
        }
    }

    m_uvRect.left = m_currentFrame * m_spriteSize;
}

void Animation::reset() {
    m_currentFrame = 0;
    m_totalTime = 0.f;
    m_uvRect.left = 0; // La première image est toujours à l'index 0
}

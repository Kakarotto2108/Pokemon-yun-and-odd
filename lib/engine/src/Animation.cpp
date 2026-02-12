#include "Animation.hpp"

Animation::Animation(int line, int frameCount, float switchTime, int spriteSize, int firstFrame)
    : m_line(line)
    , m_frameCount(frameCount)
    , m_switchTime(switchTime)
    , m_spriteSize(spriteSize)
    , m_currentFrame(0)
    , m_firstFrame(firstFrame)
    , m_totalTime(0.f)
{
    // Initialisation du premier rectangle
    m_uvRect.width = m_spriteSize;
    m_uvRect.height = m_spriteSize;
    m_uvRect.top = m_line * m_spriteSize;
    m_uvRect.left = m_firstFrame * m_spriteSize;
}

void Animation::update(float dt)
{
    m_totalTime += dt;

    if (m_totalTime < m_switchTime)
        return;

    m_totalTime -= m_switchTime;

    if (m_backToZero)
    {
        // Retour au centre
        m_currentFrame = 0;
        m_backToZero = false;

        // Préparer la prochaine cible
        m_targetFrame++;

        if (m_targetFrame >= m_frameCount)
            m_targetFrame = 1;
    }
    else
    {
        // Aller vers la frame suivante (>0)
        m_currentFrame = m_targetFrame;
        m_backToZero = true;
    }

    m_uvRect.left = (m_currentFrame + m_firstFrame) * m_spriteSize;
}


void Animation::reset() {
    m_currentFrame = 0;
    m_totalTime = 0.f;
    m_uvRect.left = m_firstFrame * m_spriteSize;
}

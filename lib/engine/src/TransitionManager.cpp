#include "TransitionManager.hpp"
#include <iostream>
#include <random>

TransitionType TransitionManager::getRandomType() {
    static const std::vector<TransitionType> types = {
        TransitionType::Distortion, TransitionType::Pixelate, TransitionType::RadialWipe,
        TransitionType::Glitch,     TransitionType::Blinds,     TransitionType::Wind,
        TransitionType::Kaleido,    TransitionType::TVShutdown, TransitionType::Drunk,
        TransitionType::BlackHole,  TransitionType::Ripple
    };

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, types.size() - 1);

    return types[dis(gen)];
}

void TransitionManager::init(sf::Vector2u windowSize) {
    m_screenQuad.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    m_screenQuad.setPosition(0, 0);

    // Compilation de chaque shader depuis la mémoire
    m_distortionShader.loadFromMemory(distortionShaderCode, sf::Shader::Fragment);
    m_pixelateShader.loadFromMemory(pixelateShader, sf::Shader::Fragment);
    m_radialWipeShader.loadFromMemory(radialWipeShader, sf::Shader::Fragment);
    m_glitchShader.loadFromMemory(glitchShader, sf::Shader::Fragment);
    m_blindsShader.loadFromMemory(blindsShader, sf::Shader::Fragment);
    m_windShader.loadFromMemory(windShader, sf::Shader::Fragment);
    m_kaleidoShader.loadFromMemory(kaleidoShader, sf::Shader::Fragment);
    m_tvShutdownShader.loadFromMemory(tvShutdownShader, sf::Shader::Fragment);
    m_drunkShader.loadFromMemory(drunkShader, sf::Shader::Fragment);
    m_blackHoleShader.loadFromMemory(blackHoleShader, sf::Shader::Fragment);
    m_rippleShader.loadFromMemory(rippleShader, sf::Shader::Fragment);
}

void TransitionManager::start(TransitionType type, float durationIn, std::function<void()> action, float durationOut) {
    m_active = true;
    m_type = type;
    m_midAction = action;
    m_durIn = durationIn;
    m_durOut = durationOut;
    m_progress = 0.f;
    m_timer = 0.f;
    m_actionExecuted = false;
}

void TransitionManager::update(float dt) {
    if (!m_active) return;

    m_timer += dt;

    if (!m_actionExecuted) {
        // PHASE D'ENTRÉE : On s'assombrit
        m_progress += dt / m_durIn;
        if (m_progress >= 1.0f) {
            m_progress = 1.0f;
            if (m_midAction) m_midAction(); // On change de zone ICI (écran noir)
            m_actionExecuted = true;
        }
    } else {
        // PHASE DE SORTIE : On s'éclaircit
        m_progress -= dt / m_durOut;
        if (m_progress <= 0.0f) {
            m_progress = 0.0f;
            m_active = false; // Fin de la transition
        }
    }
}

void TransitionManager::render(sf::RenderWindow& window, const sf::Texture& sceneTexture) {
    if (!m_active) return;

    sf::Shader* currentShader = nullptr;

    // 1. Sélection du shader selon le type
    switch (m_type) {
        case TransitionType::Distortion: currentShader = &m_distortionShader; break;
        case TransitionType::Pixelate:   currentShader = &m_pixelateShader;   break;
        case TransitionType::RadialWipe: currentShader = &m_radialWipeShader; break;
        case TransitionType::Glitch:     currentShader = &m_glitchShader;     break;
        case TransitionType::Blinds:     currentShader = &m_blindsShader;     break;
        case TransitionType::Wind:       currentShader = &m_windShader;       break;
        case TransitionType::Kaleido:    currentShader = &m_kaleidoShader;    break;
        case TransitionType::TVShutdown: currentShader = &m_tvShutdownShader; break;
        case TransitionType::Drunk:      currentShader = &m_drunkShader;      break;
        case TransitionType::BlackHole:  currentShader = &m_blackHoleShader;  break;
        case TransitionType::Ripple:     currentShader = &m_rippleShader;     break;
        case TransitionType::Fade:
            m_screenQuad.setTexture(nullptr); // Pas de texture pour un fondu simple
            m_screenQuad.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_progress * 255)));
            window.draw(m_screenQuad);
            return;
        default: return;
    }

    // 2. Envoi des paramètres communs si on utilise un shader
    if (currentShader) {
        currentShader->setUniform("texture", sceneTexture);
        currentShader->setUniform("progress", m_progress);
        currentShader->setUniform("time", m_timer);
        
        m_screenQuad.setTexture(&sceneTexture);
        m_screenQuad.setFillColor(sf::Color::White); // Reset la couleur pour ne pas teinter le shader
        
        window.draw(m_screenQuad, currentShader);
    }
}

#ifndef TRANSITION_MANAGER_HPP
#define TRANSITION_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <functional>

enum class TransitionType {
    None, Fade, Distortion, Pixelate, RadialWipe, Glitch, 
    Blinds, Wind, Kaleido, TVShutdown, Drunk, BlackHole, Ripple
};

class TransitionManager {
public:
    static TransitionManager& getInstance() {
        static TransitionManager instance;
        return instance;
    }

    TransitionType getRandomType();

    void init(sf::Vector2u windowSize);
    
    // Lance une transition : 
    // durationIn : temps pour arriver au max de l'effet
    // action : fonction à exécuter au milieu (ex: switchZone)
    // durationOut : temps pour revenir à la normale
    void start(TransitionType type, float durationIn, std::function<void()> action, float durationOut = 0.5f);

    void update(float dt);
    
    // Cette fonction dessine la scène avec l'effet par dessus
    void render(sf::RenderWindow& window, const sf::Texture& sceneTexture);

    bool isRunning() const { return m_active; }

private:
    TransitionManager() : m_active(false), m_progress(0.f) {}

    sf::Shader m_distortionShader;
    sf::Shader m_pixelateShader;
    sf::Shader m_radialWipeShader;
    sf::Shader m_glitchShader;
    sf::Shader m_blindsShader;
    sf::Shader m_windShader;
    sf::Shader m_kaleidoShader;
    sf::Shader m_tvShutdownShader;
    sf::Shader m_drunkShader;
    sf::Shader m_blackHoleShader;
    sf::Shader m_rippleShader;
    sf::RectangleShape m_screenQuad;
    
    TransitionType m_type = TransitionType::None;
    bool m_active = false;
    bool m_actionExecuted = false;
    float m_progress = 0.f; // de 0 à 1
    float m_timer = 0.f;
    float m_durIn, m_durOut;
    std::function<void()> m_midAction;

    const std::string distortionShaderCode = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;
        uniform float time;

        void main() {
            // Coordonnées UV (0.0 à 1.0)
            vec2 uv = gl_TexCoord[0].xy;
            
            // On centre les coordonnées (-0.5 à 0.5) pour pivoter autour du milieu
            vec2 centeredUv = uv - vec2(0.5, 0.5);
            
            // Calcul de la distance au centre
            float dist = length(centeredUv);
            
            // Correction ici : atan(y, x) remplace atan2(y, x) en GLSL 120
            float angle = atan(centeredUv.y, centeredUv.x);
            
            // L'intensité de la spirale : augmente avec progress
            // On multiplie par (1.0 - dist) pour que le centre tourne plus vite que les bords
            float spiralStrength = progress * 12.0; 
            float newAngle = angle + (spiralStrength * (1.0 - dist));
            
            // On repasse en coordonnées cartésiennes
            vec2 spiralUv;
            spiralUv.x = cos(newAngle) * dist + 0.5;
            spiralUv.y = sin(newAngle) * dist + 0.5;

            // Récupération du pixel
            vec4 pixel = texture2D(texture, spiralUv);
            
            // Fondu au noir
            float factor = clamp(1.0 - progress, 0.0, 1.0);
            
            gl_FragColor = vec4(pixel.rgb * factor, 1.0);
        }
    )";

    const std::string pixelateShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;

        void main() {
            float size = 0.001 + (progress * 0.1);
            vec2 uv = gl_TexCoord[0].xy;
            vec2 dividedUv = vec2(floor(uv.x / size) * size, floor(uv.y / size) * size);
            
            vec4 pixel = texture2D(texture, dividedUv);
            gl_FragColor = vec4(pixel.rgb * (1.0 - progress), 1.0);
        }
    )";

    const std::string radialWipeShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float dist = distance(uv, vec2(0.5, 0.5));
            float threshold = 1.0 - progress;
            
            vec4 pixel = texture2D(texture, uv);
            float alpha = step(dist, threshold * 0.75); // Coupe net
            gl_FragColor = vec4(pixel.rgb * alpha, 1.0);
        }
    )";

    const std::string glitchShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;
        uniform float time;

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float shift = sin(uv.y * 100.0 + time * 20.0) * 0.1 * progress;
            
            vec4 pixel = texture2D(texture, vec2(uv.x + shift, uv.y));
            gl_FragColor = vec4(pixel.rgb * (1.0 - progress), 1.0);
        }
    )";

    const std::string blindsShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;
        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float bar = fract(uv.x * 10.0); // Correction : fract au lieu de fraction
            float factor = step(progress, bar);
            vec4 pixel = texture2D(texture, uv);
            gl_FragColor = vec4(pixel.rgb * factor, 1.0);
        }
    )";

    const std::string windShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float noise = fract(sin(uv.y * 1234.5) * 4567.8);
            float threshold = progress - (noise * 0.2);
            
            vec4 pixel = texture2D(texture, uv + vec2(progress * noise, 0.0));
            float mask = step(uv.x, 1.0 - threshold);
            gl_FragColor = vec4(pixel.rgb * mask, 1.0);
        }
    )";

    const std::string kaleidoShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;

        void main() {
            vec2 uv = abs(gl_TexCoord[0].xy - 0.5);
            float angle = progress * 3.1415;
            mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
            vec2 newUv = rot * uv + 0.5;
            
            vec4 pixel = texture2D(texture, fract(newUv));
            gl_FragColor = vec4(pixel.rgb * (1.0 - progress), 1.0);
        }
    )";

    const std::string tvShutdownShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;
        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float v = abs(uv.y - 0.5);
            float threshold = clamp((1.0 - progress) * 0.5, 0.001, 0.5);
            vec4 pixel = texture2D(texture, uv);
            float mask = step(v, threshold);
            gl_FragColor = vec4(pixel.rgb * mask, 1.0);
        }
    )";

    const std::string drunkShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float offset = progress * 0.05;
            vec4 r = texture2D(texture, uv + vec2(offset, 0.0));
            vec4 g = texture2D(texture, uv);
            vec4 b = texture2D(texture, uv - vec2(offset, 0.0));
            
            gl_FragColor = vec4(r.r, g.g, b.b, 1.0) * (1.0 - progress);
        }
    )";

    const std::string blackHoleShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            vec2 dir = uv - vec2(0.5);
            float dist = length(dir);
            vec2 newUv = vec2(0.5) + dir * (1.0 + progress * dist * 10.0);
            
            vec4 pixel = texture2D(texture, newUv);
            float mask = step(dist, 0.5 * (1.0 - progress));
            gl_FragColor = vec4(pixel.rgb * mask, 1.0);
        }
    )";

    const std::string rippleShader = R"(
        #version 120
        uniform sampler2D texture;
        uniform float progress;
        uniform float time;

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float dist = distance(uv, vec2(0.5));
            float wave = sin(dist * 20.0 - time * 5.0) * 0.02 * progress;
            
            vec4 pixel = texture2D(texture, uv + (uv-0.5)*wave);
            gl_FragColor = vec4(pixel.rgb * (1.0 - progress), 1.0);
        }
    )";
};

#endif
#include "World.hpp"
#include "ZoneFactory.hpp"
#include "TransitionManager.hpp"
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

World::World() {}

Zone& World::getCurrentZone() {
    return *m_zone;
}

void World::switchZone(int id) {
    if (m_zone->getId() == id) return;
    m_zone = ZoneFactory::createZone(id);
}

int World::getCurrentZoneId() const {
    return m_zone->getId();
}

void World::draw(sf::RenderWindow& window, const WorldEntity& focus) {
    getCurrentZone().drawAll(window, focus);
}

void World::drawCharacter3D(const Character& character) {
    const sf::Sprite& sprite = character.getSprite();
    const sf::Texture* texture = sprite.getTexture();
    if (!texture) return;

    sf::Vector2f pos = character.getDrawPosition();
    sf::IntRect texRect = sprite.getTextureRect();

    // 1. Sauvegarder la matrice actuelle et extraire la position du monde
    glPushMatrix();
    
    // On se déplace à la position du joueur au sol (Z=0)
    glTranslatef(pos.x, pos.y + 48.f, 0.0f);

    // 2. Annuler la rotation de la ModelView
    // On récupère la matrice actuelle
    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);

    // On réinitialise la partie rotation (les 3x3 premières colonnes) 
    // à une matrice identité, tout en gardant la translation.
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if (i == j) m[i*4+j] = 1.0f;
            else m[i*4+j] = 0.0f;
        }
    }
    
    // On recharge la matrice modifiée
    glLoadMatrixf(m);

    // 3. Préparer les dimensions et UV
    float w = (float)texRect.width;
    float h = (float)texRect.height;
    float tw = (float)texture->getSize().x;
    float th = (float)texture->getSize().y;

    float u1 = texRect.left / tw;
    float v1 = texRect.top / th;
    float u2 = (texRect.left + texRect.width) / tw;
    float v2 = (texRect.top + texRect.height) / th;

    // 4. Dessiner le Quad (maintenant qu'on est "face écran", les axes sont simples)
    sf::Texture::bind(texture);
    glBegin(GL_QUADS);
        // On inverse le signe de la hauteur (h au lieu de -h)
        glTexCoord2f(u1, v2); glVertex3f(-w/2.f, 0.f, 0.f); // Pied gauche
        glTexCoord2f(u2, v2); glVertex3f( w/2.f, 0.f, 0.f); // Pied droite
        glTexCoord2f(u2, v1); glVertex3f( w/2.f, h,   0.f); // Tête droite
        glTexCoord2f(u1, v1); glVertex3f(-w/2.f, h,   0.f); // Tête gauche
    glEnd();

    glPopMatrix(); // On restaure la matrice originale pour ne pas casser le reste
    sf::Texture::bind(NULL);
}

void World::drawTileMap3D(const TileMap& tileMap) {
    const auto& vertices = tileMap.getVertices();
    if (vertices.empty()) return;

    sf::Texture::bind(tileMap.getTileset());
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    GLsizei stride = sizeof(Vertex3D);
    glVertexPointer(3, GL_FLOAT, stride, &vertices[0].x);
    glTexCoordPointer(2, GL_FLOAT, stride, &vertices[0].u);

    glDrawArrays(GL_QUADS, 0, (GLsizei)vertices.size());

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    sf::Texture::bind(NULL);
}

void World::drawObjSprite3D(const Obj& obj) {
    const sf::Sprite& sprite = obj.getSprite();

    const sf::Texture* texture = sprite.getTexture();
    if (!texture) return;
    sf::Vector2f pos = static_cast<sf::Vector2f>(obj.getPosition()) * 32.f;
    sf::IntRect texRect = sprite.getTextureRect();
    // 1. Sauvegarder la matrice actuelle et extraire la position du monde
    glPushMatrix();
    glTranslatef(pos.x + 16.f, pos.y + 40.f, 0.0f); 
       
    // 2. Annuler la rotation de la ModelView
    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if (i == j) m[i*4+j] = 1.0f;
            else m[i*4+j] = 0.0f;
        }
    }
    glLoadMatrixf(m);
    glScalef(3.0f, 3.0f, 1.0f);
    // 3. Préparer les dimensions et UV
    float w = (float)texRect.width;
    float h = (float)texRect.height;
    float tw = (float)texture->getSize().x;
    float th = (float)texture->getSize().y;
    float u1 = texRect.left / tw;
    float v1 = texRect.top / th;
    float u2 = (texRect.left + texRect.width) / tw;
    float v2 = (texRect.top + texRect.height) / th;
    // 4. Dessiner le Quad (maintenant qu'on est "face écran", les axes sont simples)
    sf::Texture::bind(texture);
    glBegin(GL_QUADS);
        // On inverse le signe de la hauteur (h au lieu de -h)
        glTexCoord2f(u1, v2); glVertex3f(-w/2.f, 0.f, 0.f); // Pied gauche
        glTexCoord2f(u2, v2); glVertex3f( w/2.f, 0.f, 0.f); // Pied droite
        glTexCoord2f(u2, v1); glVertex3f( w/2.f, h,   0.f); // Tête droite
        glTexCoord2f(u1, v1); glVertex3f(-w/2.f, h,   0.f); // Tête gauche
    glEnd();
    glPopMatrix();
    sf::Texture::bind(NULL);
}

void World::renderEntities(Zone& zone) {
    // 1. Créer une liste de pointeurs vers tout ce qui doit être dessiné
    std::vector<WorldEntity*> entitiesToDraw;
    
    entitiesToDraw.push_back(&Player::getInstance());
    for (auto& entity : zone.getEntities()) {
        entitiesToDraw.push_back(entity.get());
    }

    // 2. Trier par coordonnée Y (plus le Y est petit, plus c'est loin)
    // On trie du Y le plus petit au Y le plus grand
    std::sort(entitiesToDraw.begin(), entitiesToDraw.end(), [](WorldEntity* a, WorldEntity* b) {
        return a->getPosition().y < b->getPosition().y;
    });

    // 3. Dessiner dans l'ordre trié
    for (WorldEntity* entity : entitiesToDraw) {
        Character* npc = dynamic_cast<Character*>(entity);
        if (npc) {
            drawCharacter3D(*npc);
        } else {
            Obj* obj = dynamic_cast<Obj*>(entity);
            if (obj) {
                drawObjSprite3D(*obj);
            }
        }
    }
}

void World::draw3D(sf::RenderTarget& target) {
    Zone& zone = getCurrentZone();
    
    // 1. Préparation des états OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Indispensable pour que les pieds des sprites ne "coupent" pas le sol
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    // 2. Rendu des couches
    drawTileMap3D(zone.getTileMap());
    renderEntities(zone);

    // 3. Nettoyage
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
}

void World::init() {
    m_zone = ZoneFactory::createZone(1);
}

void World::update(Player& player) {
    // 1. Récupérer la zone actuelle
    Zone& zone = getCurrentZone();
    sf::Vector2i pos = player.getPosition();

    // --- DEBUG ---
    int zoneWidth = zone.getWidth();
    int zoneHeight = zone.getHeight();
    int mapSize = zone.getCollisionMap().size();
    int accessIndex = pos.x + pos.y * zoneWidth;

    if (pos.x < 0 || pos.x >= zoneWidth || pos.y < 0 || pos.y >= zoneHeight || accessIndex >= mapSize) {
        std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        std::cerr << "!! PLAYER IS OUT OF BOUNDS FOR THE ZONE !!" << std::endl;
        std::cerr << "!! Player pos: (" << pos.x << ", " << pos.y << ")" << std::endl;
        std::cerr << "!! Zone size: " << zoneWidth << "x" << zoneHeight << std::endl;
        std::cerr << "!! Collision map size: " << mapSize << ", Access index: " << accessIndex << std::endl;
        std::cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        return; // On empêche le crash
    }
    // --- FIN DEBUG ---

    // 2. Vérifier si la case actuelle est une case de collision/transition
    int tileValue = zone.getCollisionMap()[accessIndex];

    // 3. Si la valeur correspond à une transition
    if (tileValue > 0) { // Note: tileValue est un int, la comparaison avec 0.f marche mais c'est inhabituel
        int targetZoneId = tileValue >> 16;
        int targetSpawnIndex = tileValue & 0xFFFF;

        // Si on n'est pas déjà en train de changer de zone, on lance la transition
        if (!TransitionManager::getInstance().isRunning()) {
            // On récupère un type aléatoire ici
            TransitionType randomEffect = TransitionManager::getInstance().getRandomType();

            TransitionManager::getInstance().start(randomEffect, 0.8f, [=, &player](){
                switchZone(targetZoneId);
                player.stopAnimation();
                player.setLogicalPos(getCurrentZone().getSpawnPos(targetSpawnIndex));
            }, 0.5f);
        }
    }
}

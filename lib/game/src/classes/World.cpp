#include "World.hpp"
#include "ZoneFactory.hpp"
#include "TransitionManager.hpp"
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "Event.hpp"
#include <fstream>

Zone& World::getCurrentZone() {
    return *m_zone;
}

void World::switchZone(int id) {
    // if (m_zone->getId() == id) return;
    
    m_zone = ZoneFactory::createZone(id);
}

int World::getCurrentZoneId() const {
    return m_zone->getId();
}

void World::drawCharacter3D(const Character& character) {
    const sf::Sprite& sprite = character.getSprite();
    const sf::Texture* texture = sprite.getTexture();
    if (!texture) return;

    sf::Vector2f pos = character.getDrawPosition();
    sf::IntRect texRect = sprite.getTextureRect();

    // --- RECALCUL DES DIMENSIONS ET UV ---
    float w = (float)texRect.width;
    float h = (float)texRect.height;
    float tw = (float)texture->getSize().x;
    float th = (float)texture->getSize().y;

    float u1 = texRect.left / tw;
    float v1 = texRect.top / th;
    float u2 = (texRect.left + texRect.width) / tw;
    float v2 = (texRect.top + texRect.height) / th;

    glPushMatrix();
    
    // On positionne le sprite. Le Z est légèrement ajusté par le Y pour éviter le Z-fighting
    glTranslatef(pos.x, pos.y + 48.f, pos.y * 0.0001f);

    // Billboard : on annule la rotation de la caméra
    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if (i == j) m[i*4+j] = 1.0f;
            else m[i*4+j] = 0.0f;
        }
    }
    glLoadMatrixf(m);

    // Dessin
    sf::Texture::bind(texture);
    glBegin(GL_QUADS);
        glTexCoord2f(u1, v2); glVertex3f(-w/2.f, 0.f, 0.f); 
        glTexCoord2f(u2, v2); glVertex3f( w/2.f, 0.f, 0.f);
        glTexCoord2f(u2, v1); glVertex3f( w/2.f, h,   0.f); 
        glTexCoord2f(u1, v1); glVertex3f(-w/2.f, h,   0.f);
    glEnd();

    glPopMatrix();
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
    glScalef(sprite.getScale().x, sprite.getScale().y, 1.0f);
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

void World::drawIogSprite3D(const Iog& iog) {
    const sf::Sprite& sprite = iog.getSprite();
    const sf::Texture* texture = sprite.getTexture();
    if (!texture) return;

    sf::Vector2f pos = static_cast<sf::Vector2f>(iog.getPosition()) * 32.f;
    
    sf::IntRect texRect = sprite.getTextureRect();
    if (texRect.width == 0 || texRect.height == 0) {
        texRect = sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y);
    }

    glPushMatrix();
    // On positionne l'objet au milieu de sa case (x+16) et sur le sol (y+32)
    glTranslatef(pos.x + 16.f, pos.y + 32.f, 0.0f); 
       
    // Logique "Billboard" pour que le sprite fasse toujours face à la caméra
    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if (i == j) m[i*4+j] = 1.0f;
            else m[i*4+j] = 0.0f;
        }
    }
    glLoadMatrixf(m);
    
    glScalef(sprite.getScale().x, sprite.getScale().y, 1.0f);

    float w = (float)texRect.width;
    float h = (float)texRect.height;
    float u1 = (float)texRect.left / texture->getSize().x;
    float v1 = (float)texRect.top / texture->getSize().y;
    float u2 = (float)(texRect.left + texRect.width) / texture->getSize().x;
    float v2 = (float)(texRect.top + texRect.height) / texture->getSize().y;

    sf::Texture::bind(texture);
    glBegin(GL_QUADS);
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
            Iog* iog = dynamic_cast<Iog*>(entity);
            if (iog) {
                drawIogSprite3D(*iog);
            } else {
                Obj* obj = dynamic_cast<Obj*>(entity);
                if (obj) {
                    drawObjSprite3D(*obj);
                }
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
    GameEvents::OnEntityDestroyed.subscribe([this](WorldEntity* e) {
        m_zone->removeEntity(e);
    });
}

void World::update(float dt, Player& player) {
    // 1. Récupérer la zone actuelle
    Zone& zone = getCurrentZone();
    sf::Vector2i pos = player.getPosition();

    // Mise à jour de toutes les entités de la zone (PNJ, etc.)
    player.update(dt, zone);
    for (auto& entity : zone.getEntities()) {
        Character* character = dynamic_cast<Character*>(entity.get());
        if (character) {
            character->update(dt, zone);
        }   
    }

    // 2. Calcul de l'index pour la collision/transition
    // On transforme les coordonnées (x, y) en index 1D pour le tableau
    int mapWidth = zone.getWidth();
    int accessIndex = pos.y * mapWidth + pos.x;

    // Sécurité : on vérifie que l'index est bien dans les limites du tableau
    const std::vector<int>& collisionMap = zone.getCollisionMap();
    if (accessIndex >= 0 && accessIndex < static_cast<int>(collisionMap.size())) {
        
        int tileValue = collisionMap[accessIndex];

        // 3. Si la valeur correspond à une transition (tileValue > 0)
        if (tileValue > 0) {
            // Extraction des données stockées dans l'entier (ID zone et Index de spawn)
            int targetZoneId = tileValue >> 16;
            int targetSpawnIndex = tileValue & 0xFFFF;

            // Si on n'est pas déjà en train de changer de zone, on lance la transition
            if (!TransitionManager::getInstance().isRunning()) {
                TransitionType randomEffect = TransitionManager::getInstance().getRandomType();

                TransitionManager::getInstance().start(randomEffect, 0.8f, [this, targetZoneId, targetSpawnIndex, &player](){
                    // Changement effectif de zone
                    GameInstance::getInstance().saveZoneState(m_zone->getId(), m_zone->getEntities());
                    this->switchZone(targetZoneId);
                    
                    // On arrête le mouvement du joueur et on le téléporte au spawn de la nouvelle zone
                    player.stopAnimation();
                    player.setLogicalPos(this->getCurrentZone().getSpawnPos(targetSpawnIndex));
                }, 0.5f);
            }
        }
    }
}

#include "PlayerController.hpp"
#include "Interactable.hpp"
#include "DialogManager.hpp"
#include "TransitionManager.hpp"
#include "GameInstance.hpp"
#include "PokemonInstance.hpp"
#include "Menu.hpp"
#include "Bag.hpp"
#include "TeamDisplay.hpp"
#include <iostream>

PlayerController* PlayerController::s_instance = nullptr;

PlayerController* PlayerController::getInstance() {
    return s_instance;
}

void PlayerController::create(World& world, Player& player) {
    if (!s_instance)
        s_instance = new PlayerController(world, player);
}

void PlayerController::destroy() {
    delete s_instance;
    s_instance = nullptr;
}

PlayerController::PlayerController(World& world, Player& player) : m_world(world), m_player(player) {
    auto isChoiceBoxVisible = []() {
        return Menu::getInstance().isVisible() ||
               Bag::getInstance().getChoiceBox().isVisible() ||
               TeamDisplay::getInstance().getChoiceBox().isVisible() ||
               DialogManager::getInstance().getChoiceBox().isVisible();
    };

    auto isInputLocked = [isChoiceBoxVisible]() {
        return DialogManager::getInstance().isActive() ||
               TransitionManager::getInstance().isRunning() ||
               isChoiceBoxVisible();
    };

    Controller::getInstance().onAxisChanged("MoveHorizontal", [this, isInputLocked](float val) {
        if (isInputLocked()) {
            m_hAxis = 0.f;
            return;
        }
        m_hAxis = val;
    });
    Controller::getInstance().onAxisChanged("MoveVertical", [this, isInputLocked](float val) {
        if (isInputLocked()) {
            m_vAxis = 0.f;
            return;
        }
        m_vAxis = val;
    });

    Controller::getInstance().onActionPressed("Interact", [this, isChoiceBoxVisible]() {
        // On bloque toute interaction pendant une transition
        if (TransitionManager::getInstance().isRunning()) return;

        if(Player::getInstance().getFrame() == "ReceiveItem2"){
            std::vector<std::string> lstAnim = {"ReceiveItem3", "WalkDown"};
            Player::getInstance().startAnimation(lstAnim);
        }
        
        if (DialogManager::getInstance().isActive()) {
            DialogManager::getInstance().next();
        } else if (!isChoiceBoxVisible()) { // Ne pas interagir avec la map si une boite de choix est visible
            sf::Vector2i front = m_player.getFacingTile();
            Zone& currentZone = m_world.getCurrentZone();
            Interactable* interactable = dynamic_cast<Interactable*>(currentZone.getEntityAt(front.x, front.y));

            if (interactable)
            {
                interactable->interact();
            }
        }
    });
        Controller::getInstance().onActionPressed("Load", [this, isInputLocked]() {
            
            if (isInputLocked()) {
                return;
            }
            try {
                GameInstance::getInstance().loadFromFileEncrypted("savegame.dat");
                // Après le chargement, on doit recharger la zone actuelle
                int currentZoneId = m_world.getCurrentZoneId();
                m_world.switchZone(currentZoneId);
            } catch (const std::exception& e) {
                std::cerr << "Failed to load game: " << e.what() << std::endl;
            }
        });
    }

void PlayerController::update(Zone& zone, float dt) {
    if (!m_inputEnabled) return;
    if (m_moveTimer.getElapsedTime().asSeconds() < m_player.getMoveDelay() && m_player.getIsMoving()) return;

    // Gestion de la course (Touche B / Run)
    if (Controller::getInstance().isActionActive("Run") && m_player.getInventory().getQuantity(ItemDatabase::getInstance().getItem("Ch. de Sport")) > 0) {
        m_player.setRunning(true);
        m_player.setMoveDelay(0.1f); // Plus rapide (0.1s par case au lieu de 0.2s)
    } else {
        m_player.setRunning(false);
        m_player.setMoveDelay(0.2f); // Vitesse normale
    }

    sf::Vector2i direction(0, 0);

    // Priorité à l'axe horizontal (ou vertical selon ton choix))
    direction.x = (int)m_hAxis;
    if (m_hAxis == 0.f) {
        direction.y = (int)m_vAxis;
    }
    
    m_player.moveRequest(direction, zone);
    m_moveTimer.restart();
}
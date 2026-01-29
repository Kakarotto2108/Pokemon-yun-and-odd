#include "Npc.hpp"
#include "ScriptManager.hpp"
#include "DialogManager.hpp"
#include "CharacterPath.hpp"    
#include "Player.hpp"
#include <iostream>

Npc::Npc(const std::string& name, const std::string& sprite, sf::Vector2i pos, 
         int orientation, const std::string& dialogueKey, std::unique_ptr<CharacterPath> path, const Inventory& inventory)
    : Character(name, sprite, pos, orientation, std::move(path), inventory), 
      m_dialogueKey(dialogueKey) {}

void Npc::interact() {
    setOrientation(3 - Player::getInstance().getOrientation());   
    m_path->pause();
    const auto& script = ScriptManager::getInstance().getDialogue(m_dialogueKey);
    DialogManager::getInstance().startDialogue(script, this, [this]() {
        if (hasPath()) {
            m_path->start();
        }
    });
}
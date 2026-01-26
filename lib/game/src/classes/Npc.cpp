#include "Npc.hpp"
#include "ScriptManager.hpp"
#include "DialogManager.hpp"
#include "Player.hpp"
#include <iostream>

Npc::Npc(const std::string& name, const std::string& sprite, sf::Vector2i pos, 
         int orientation, const std::string& dialogueKey)
    : Character(name, sprite, pos, orientation), 
      m_dialogueKey(dialogueKey) {}

void Npc::interact() {    
    const auto& script = ScriptManager::getInstance().getDialogue(m_dialogueKey);
    DialogManager::getInstance().startDialogue(script);
}
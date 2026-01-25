#include "Npc.hpp"

Npc::Npc(const std::string& name, const std::string& sprite, sf::Vector2i pos, 
         int orientation, const std::string& dialogue, Item& item)
    : Character(name, sprite, pos, orientation), 
      m_dialogue(dialogue) {}

void Npc::interact() {
    std::cout << m_name << " dit : " << m_dialogue << std::endl;
}
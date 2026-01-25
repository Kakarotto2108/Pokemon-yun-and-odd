#include "Npc.hpp"
#include "DialogManager.hpp"
#include "Player.hpp"
#include <iostream>

Npc::Npc(const std::string& name, const std::string& sprite, sf::Vector2i pos, 
         int orientation, const std::string& dialogue, Item& item)
    : Character(name, sprite, pos, orientation), 
      m_dialogue(dialogue) {}

void Npc::interact() {
    std::vector<DialogueStep> script;

    script.push_back({"Oh, tiens ! Prends ceci.", BoxType::Classic});

    DialogueStep giveStep;
    giveStep.text = "$[white]Vous recevez une $[blue]Pokéball $[white]!";
    giveStep.type = BoxType::Object;
    giveStep.action = [this]() {
        Player::getInstance().getInventory().addItem(Item(
            "Poké ball", 
            ItemPocket::Balls, 
            "Un objet...", 
            true
        ),1); 
    };
    
    script.push_back(giveStep);
    script.push_back({"Fais-en bon usage !", BoxType::Classic});

    DialogManager::getInstance().startDialogue(script);
}
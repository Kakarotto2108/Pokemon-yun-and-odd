#include "ItemGround.hpp"
#include "ScriptManager.hpp"
#include "DialogManager.hpp"
#include "Player.hpp"
#include "Event.hpp"
#include <iostream>

Iog::Iog(std::string name,sf::Vector2i pos)
    : WorldEntity(name, pos), m_name(std::move(name)) {
        std::string path = "assets/sprite/overworld/overworld_pokeball.png";
        m_sprite.setTexture(TextureManager::getInstance().get(path));
        
        // Positionnement graphique pour le rendu 3D billboard
        m_sprite.setOrigin(8.f, 16.f); 
        m_sprite.setPosition(pos.x * 32.f + 16.f, pos.y * 32.f + 16.f); 
        m_sprite.setScale(1.5f, 1.5f);
    }

    void Iog::interact() {    
        std::string itemName = m_name; // On capture pour le lambda

        DialogueStep step1;
        step1.text = "Vous avez trouvé : " + itemName + " !";
        step1.type = BoxType::Object;
        step1.action = [itemName, this]() {
            Item item(itemName, ItemPocket::Items, "Objet trouvé au sol.");
            Player::getInstance().getInventory().addItem(item, 1); 
            GameEvents::OnEntityDestroyed.notify(this);
        };

        const std::vector<DialogueStep> script = { step1 };
        DialogManager::getInstance().startDialogue(script, this);

    }

void Iog::draw(sf::RenderWindow& window) const {
}

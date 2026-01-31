#include "ItemGround.hpp"
#include "DialogManager.hpp"
#include "Player.hpp"
#include "Event.hpp"

Iog::Iog(const std::string& name, sf::Vector2i pos)
    : Obj(
        name,
        "assets/sprite/overworld/overworld_pokeball.png", // texture
        pos,
        "iog_dialogue",                                   // clé dialogue
        std::nullopt
                                       // pas d'item stocké dans Obj
      )
{
    setSize(1.5f, 1.5f);
}


void Iog::interact()
{
    std::string itemName = getName(); // si WorldEntity expose un getter

    DialogueStep step1;
    step1.text = "Vous avez trouvé : " + itemName + " !";
    step1.type = BoxType::Object;
    Item item(itemName, ItemPocket::Items, "Objet trouvé au sol.");
    Player::getInstance().getInventory().addItem(item, 1);
    GameEvents::OnEntityDestroyed.notify(this);
    Player::getInstance().getInventory().debugPrint();
    const std::vector<DialogueStep> script = { step1 };
    DialogManager::getInstance().startDialogue(script, this);
}

EntityState Iog::getState() const {
    EntityState state = Obj::getState();
    state.type = EntityType::IOG;
    return state;
}

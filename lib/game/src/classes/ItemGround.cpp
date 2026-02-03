#include "ItemGround.hpp"
#include "DialogManager.hpp"
#include "Player.hpp"
#include "Event.hpp"

Iog::Iog(const std::string& name, sf::Vector2i pos)
    : Obj(
        name,
        "assets/sprite/overworld/overworld_pokeball.png", // texture
        pos,
        "iog_dialogue",
        sf::Vector2f(1.f, 1.f),
        std::nullopt                                      // pas d'item stocké dans Obj
      )
{
    setSize(1.5f, 1.5f);
}


void Iog::interact()
{
    std::string itemName = getName(); // si WorldEntity expose un getter

    DialogueStep step1;
    step1.text = Player::getInstance().getName() + " trouve : $[blue]" + itemName + " $[white]!";
    step1.type = BoxType::Object;
    Item item(itemName, ItemPocket::Items, "Objet trouvé au sol.");
    Player::getInstance().getInventory().addItem(item, 1);
    GameEvents::OnEntityDestroyed.notify(this);
    DialogueStep step2;
    step2.text = Player::getInstance().getName() + " a mis l'objet " + itemName + " \ndans la Poche $[blue]" + toString(item.getPocket()) + " $[white].";
    step2.type = BoxType::Object;
    const std::vector<DialogueStep> script = { step1, step2 };
    DialogManager::getInstance().startDialogue(script, this);
    std::vector<std::string> lstAnim = {"ReceiveItem1", "ReceiveItem2"};
    Player::getInstance().startAnimation(lstAnim);
}

EntityState Iog::getState() const {
    EntityState state = Obj::getState();
    state.type = EntityType::IOG;
    return state;
}

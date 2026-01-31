#include "EventManager.hpp"
#include "World.hpp"
#include "GameInstance.hpp"
#include "DialogManager.hpp"

EventManager::EventManager() {
    GameEvents::SaveGame.subscribe([]() {
        GameInstance::getInstance().saveZoneState(World::getInstance().getCurrentZoneId(), World::getInstance().getCurrentZone().getEntities());
        GameInstance::getInstance().saveToFileEncrypted("savegame.dat");
        DialogueStep step1;
        step1.text = "Sauvegarde effectuée !";
        step1.type = BoxType::Classic;
        const std::vector<DialogueStep> script = {
            step1
        };
        DialogManager::getInstance().startDialogue(script);

    });
}


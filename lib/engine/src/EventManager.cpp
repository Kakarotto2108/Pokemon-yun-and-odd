#include "EventManager.hpp"
#include "World.hpp"
#include "GameInstance.hpp"
#include "DialogManager.hpp"
#include "GameChoiceBox.hpp"
#include "Npc.hpp"
#include "Interactable.hpp"
#include "Bag.hpp"

EventManager::EventManager() {
    GameEvents::SaveGame.subscribe([]() {
        DialogueStep step1;
        step1.text = "Voulez-vous sauvegarder la partie ?";
        step1.type = BoxType::Classic;
        const std::vector<DialogueStep> script = {
            step1
        };

        DialogManager::getInstance().startDialogue(script);
        std::vector<std::pair<std::string, std::string>> choices = {
            {"Oui", "YesChoice"},
            {"Non", "NoChoice"}
        };
        GameChoiceBox::getInstance().init(choices);
        GameChoiceBox::getInstance().setChoiceIndex(0);
        GameChoiceBox::getInstance().setVisible(true);
        GameEvents::YesChoice.subscribe([]() {
            GameInstance::getInstance().saveZoneState(World::getInstance().getCurrentZoneId(), World::getInstance().getCurrentZone().getEntities());
            GameInstance::getInstance().saveToFileEncrypted("savegame.dat");
            DialogueStep step2;
            step2.text = "Sauvegarde effectuée !";
            step2.type = BoxType::Classic;
            const std::vector<DialogueStep> script = {
                step2
            };

            DialogManager::getInstance().startDialogue(script);
        });
        


    });
    GameEvents::Ev1.subscribe([this]() {
        makeChoice("DIAG_1_1");
    });
    GameEvents::Ev2.subscribe([this]() {
        makeChoice("DIAG_1_2");
    });
    GameEvents::Ev3.subscribe([this]() {
        makeChoice("DIAG_1_3");
    });

    GameEvents::OpenBag.subscribe([]() {
        Bag::getInstance().open();
    });
}

void EventManager::makeChoice(std::string dialoguekey) {
    WorldEntity* speaker = DialogManager::getInstance().getCurrentSpeaker();
        Npc* npc = dynamic_cast<Npc*>(speaker);
        if (npc) {
            npc->setDialogueKey(dialoguekey);
            Interactable* interactable = dynamic_cast<Interactable*>(speaker);
            if (interactable) {
                interactable->interact();
            }
        }
    }   

#include "EventManager.hpp"
#include "World.hpp"
#include "GameInstance.hpp"
#include "DialogManager.hpp"
#include "GameChoiceBox.hpp"
#include "Npc.hpp"
#include "Interactable.hpp"
#include "Bag.hpp"
#include "Menu.hpp"
#include "PokemonInstance.hpp"
#include "TeamDisplay.hpp"

namespace {
    enum class YesNoContext {
        NONE,
        SAVE_GAME
    };
    YesNoContext g_currentYesNoContext = YesNoContext::NONE;
}


EventManager::EventManager() {
    GameEvents::SaveGame.subscribe([]() {
        DialogueStep step1;
        if (GameInstance::getInstance().saveFileExists("savegame.dat")) {
            step1.text = "Une sauvegarde existe déjà. L'écraser ?";
        } else {
            step1.text = "Voulez-vous sauvegarder la partie ?";
        }
        step1.type = BoxType::Classic;

        DialogManager::getInstance().startDialogue({step1});

        std::vector<std::pair<std::string, std::string>> choices = {
            {"Oui", "YesChoice"},
            {"Non", "NoChoice"}
        };
        GameChoiceBox::getInstance().init(choices);
        GameChoiceBox::getInstance().setChoiceIndex(0);
        GameChoiceBox::getInstance().setVisible(true);
        g_currentYesNoContext = YesNoContext::SAVE_GAME;
    });

    GameEvents::YesChoice.subscribe([]() {
        if (g_currentYesNoContext == YesNoContext::SAVE_GAME) {
            GameInstance::getInstance().saveZoneState(World::getInstance().getCurrentZoneId(), World::getInstance().getCurrentZone().getEntities());
            GameInstance::getInstance().saveToFileEncrypted("savegame.dat");
            DialogManager::getInstance().startDialogue({{"Sauvegarde effectuée !", BoxType::Classic}});
            g_currentYesNoContext = YesNoContext::NONE;
        }
    });

    GameEvents::NoChoice.subscribe([]() {
        if (g_currentYesNoContext == YesNoContext::SAVE_GAME) {
            DialogManager::getInstance().setActive(false);
            Menu::getInstance().open();
            g_currentYesNoContext = YesNoContext::NONE;
        }
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

    GameEvents::OpenPokemon.subscribe([]() {
        if (TeamDisplay::getInstance().m_team.empty()) {
            PokemonInstance pkm("Moustillon", 5);
            TeamDisplay::getInstance().addPokemon(pkm);
            PokemonInstance pkm2("Pikachu", 7);
            TeamDisplay::getInstance().addPokemon(pkm2);
        }
        TeamDisplay::getInstance().open();
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

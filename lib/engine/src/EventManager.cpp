#include "EventManager.hpp"
#include "World.hpp"
#include "GameInstance.hpp"
#include "DialogManager.hpp"
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
        DialogManager::getInstance().setChoiceBox(choices);
        DialogManager::getInstance().setChoiceBoxVisible(true);
        Menu::getInstance().close();
        g_currentYesNoContext = YesNoContext::SAVE_GAME;
    });

    GameEvents::YesChoice.subscribe([]() {
        if (g_currentYesNoContext == YesNoContext::SAVE_GAME) {
            GameInstance::getInstance().saveZoneState(World::getInstance().getCurrentZoneId(), World::getInstance().getCurrentZone().getEntities());
            GameInstance::getInstance().saveToFileEncrypted("savegame.dat");
            DialogManager::getInstance().startDialogue({{"Sauvegarde effectuée !", BoxType::Classic}});
            g_currentYesNoContext = YesNoContext::NONE;
        }
        DialogManager::getInstance().setActive(false);
        DialogManager::getInstance().setChoiceBoxVisible(false);
    });

    GameEvents::NoChoice.subscribe([]() {
        if (g_currentYesNoContext == YesNoContext::SAVE_GAME) {
            DialogManager::getInstance().setActive(false);
            Menu::getInstance().open();
            g_currentYesNoContext = YesNoContext::NONE;
        }
        DialogManager::getInstance().setActive(false);
        DialogManager::getInstance().setChoiceBoxVisible(false);
    });

    GameEvents::Ev1.subscribe([this]() {
        makeChoice("DIAG_1_1");
        DialogManager::getInstance().setChoiceBoxVisible(false);
    });

    GameEvents::Ev2.subscribe([this]() {
        makeChoice("DIAG_1_2");
        DialogManager::getInstance().setChoiceBoxVisible(false);
    });

    GameEvents::Ev3.subscribe([this]() {
        makeChoice("DIAG_1_3");
        DialogManager::getInstance().setChoiceBoxVisible(false);
    });

    GameEvents::OpenBag.subscribe([]() {
        Bag::getInstance().open();
        Menu::getInstance().close();
    });

    GameEvents::OpenPokemon.subscribe([]() {
        if (TeamDisplay::getInstance().m_team.empty()) {
            PokemonInstance pkm("Moustillon", 5);
            TeamDisplay::getInstance().addPokemon(pkm);
            PokemonInstance pkm2("Pikachu", 7);
            TeamDisplay::getInstance().addPokemon(pkm2);
        }
        TeamDisplay::getInstance().open();
        Menu::getInstance().setFocus(false);
        Menu::getInstance().close();
    });

    GameEvents::ViewPokemon.subscribe([]() {
        if (!TeamDisplay::getInstance().getSummary()) {
            TeamDisplay::getInstance().m_subChoiceBox.open();
        }
    });

    GameEvents::SummaryChoice.subscribe([]() {
        TeamDisplay::getInstance().setSummary(true);
        TeamDisplay::getInstance().m_subChoiceBox.setVisible(false);
        TeamDisplay::getInstance().m_choiceBox.setFocus(true);
        Menu::getInstance().setFocus(false);
        Menu::getInstance().close();
    });

    GameEvents::MoveOrder.subscribe([](const std::string& choice) {
        TeamDisplay::getInstance().addSwitchChoice(choice);
        if (TeamDisplay::getInstance().getSwitchChoice().size() > 1) {
            TeamDisplay::getInstance().switchChoice(true);
        }
    });

    GameEvents::OrderChoice.subscribe([](const std::string& choice) {
        if (TeamDisplay::getInstance().m_subChoiceBox.isVisible()) {
            TeamDisplay::getInstance().addSwitchChoice(TeamDisplay::getInstance().m_choiceBox.getChoiceName());
            TeamDisplay::getInstance().m_subChoiceBox.setVisible(false);
            TeamDisplay::getInstance().m_choiceBox.setFocus(true);
            std::vector<std::pair<std::string, std::string>> choices;
            for (const auto& pkm : TeamDisplay::getInstance().m_team) {
                choices.emplace_back(pkm.m_surname, "OrderChoice");
            }
            
            size_t teamSize = TeamDisplay::getInstance().m_team.size();

            if (teamSize <= 6) {
                for (size_t i = 0; i < 6 - teamSize; ++i) {
                    choices.emplace_back("---", "EmptySlot");
                }
            }
            TeamDisplay::getInstance().m_choiceBox.init(choices);
        } else {
            TeamDisplay::getInstance().addSwitchChoice(choice);
            if (TeamDisplay::getInstance().getSwitchChoice().size() > 1) {
                TeamDisplay::getInstance().switchChoice(false);
            }
        }
    });

    GameEvents::ItemsChoice.subscribe([]() {
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

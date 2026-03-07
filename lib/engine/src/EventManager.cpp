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
#include "Player.hpp"

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
        std::vector<std::pair<std::string, std::string>> choices = {
            {"Donner", "SelectGiveItem"},
            {"Prendre", "TakeItem"},
            {"Echanger", "SwitchItem"},
            {"Retour", "Cancel"}
        };
        TeamDisplay::getInstance().m_subChoiceBox.init(choices);
        TeamDisplay::getInstance().m_subChoiceBox.reset();
    });

    GameEvents::SelectGiveItem.subscribe([]() {
        Bag::getInstance().lookingForItem = true;
        Bag::getInstance().open();
        Menu::getInstance().close();
        TeamDisplay::getInstance().m_subChoiceBox.setVisible(false);
        TeamDisplay::getInstance().m_choiceBox.setVisible(false);
        TeamDisplay::getInstance().m_pocketDialog.hide();
        TeamDisplay::getInstance().resetSubChoiceBox();
    });

    GameEvents::GiveItem.subscribe([](const std::string& itemName) {
        std::string selectedItemName = itemName.substr(0, itemName.size() - 3);
        std::string selectedPkm = TeamDisplay::getInstance().m_choiceBox.getChoiceName();
        std::string dialogue = "";
        if (selectedPkm.size() <= 3 || selectedPkm == "Retour") {
            return;
        }
        for (auto& pkm : TeamDisplay::getInstance().m_team) {
            if (pkm.m_surname == selectedPkm) {
                if (pkm.m_item.empty()) {
                    pkm.m_item = selectedItemName;
                    dialogue = pkm.m_surname + " tient " + selectedItemName + " !";
                    Item item = ItemDatabase::getInstance().getItem(selectedItemName);
                    Player::getInstance().getInventory().removeItem(item, 1);
                    break;
                } else {
                    dialogue = pkm.m_surname + " tient déjà l'objet " + pkm.m_item + " . Voulez-vous échanger les deux objets ?";
                    std::vector<std::pair<std::string, std::string>> choices = {
                        {"Oui", "YesGiveItem"},
                        {"Non", "Cancel"}
                    };
                    DialogManager::getInstance().setChoiceBox(choices);
                    DialogManager::getInstance().setChoiceBoxVisible(true);
                    DialogManager::getInstance().getChoiceBox().setPosition({280.f, 145.f});
                    Menu::getInstance().close();
                    break;
                }
            }
        }
        Bag::getInstance().lookingForItem = false;
        Bag::getInstance().close();
        Menu::getInstance().close();
        TeamDisplay::getInstance().m_subChoiceBox.setVisible(false);
        TeamDisplay::getInstance().m_choiceBox.open();
        TeamDisplay::getInstance().m_pocketDialog.show();
        DialogManager::getInstance().startDialogue({{dialogue, BoxType::Classic}});
    });

    GameEvents::YesGiveItem.subscribe([]() {
        std::string selectedPkm = TeamDisplay::getInstance().m_choiceBox.getChoiceName();
        std::string dialogue = "";
        for (auto& pkm : TeamDisplay::getInstance().m_team) {
            if (pkm.m_surname == selectedPkm) {
                Item olditem = ItemDatabase::getInstance().getItem(pkm.m_item);
                Player::getInstance().getInventory().addItem(olditem, 1);
                std::string itemName = Bag::getInstance().getChoiceBox().getChoiceName();
                std::string selectedItemName = itemName.substr(0, itemName.size() - 3);
                Item item = ItemDatabase::getInstance().getItem(selectedItemName);
                Player::getInstance().getInventory().removeItem(item, 1);
                pkm.m_item = selectedItemName;
                dialogue = pkm.m_surname + " tient " + pkm.m_item + " !";
                break;
            }
        }
        DialogManager::getInstance().setChoiceBoxVisible(false);
        DialogManager::getInstance().startDialogue({{dialogue, BoxType::Classic}});
    });

    GameEvents::TakeItem.subscribe([]() {
        std::string selectedPkm = TeamDisplay::getInstance().m_choiceBox.getChoiceName();
        std::string dialogue = "";
        if (selectedPkm.size() <= 3 || selectedPkm == "Retour") {
            return;
        }
        for (auto& pkm : TeamDisplay::getInstance().m_team) {
            if (pkm.m_surname == selectedPkm) {
                if (pkm.m_item.empty()) {
                    dialogue = pkm.m_surname + " ne tient rien.";
                } else {
                    Item item = ItemDatabase::getInstance().getItem(pkm.m_item);
                    Player::getInstance().getInventory().addItem(item, 1);
                    dialogue = pkm.m_surname + " vous a remis l'objet " + pkm.m_item + " !";
                    pkm.m_item = "";
                }
                break;
            }
        }
        TeamDisplay::getInstance().m_subChoiceBox.setVisible(false);
        TeamDisplay::getInstance().m_choiceBox.open();
        TeamDisplay::getInstance().resetSubChoiceBox();
        DialogManager::getInstance().startDialogue({{dialogue, BoxType::Classic}});
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

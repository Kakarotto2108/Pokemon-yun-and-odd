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

        std::vector<Choice> choices = {
            {"Oui", "YesChoice", std::monostate()},
            {"Non", "NoChoice", std::monostate()}
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
        Bag::getInstance().getChoiceBox().setFocus(true);
        Menu::getInstance().close();
    });

    GameEvents::OpenPokemon.subscribe([]() {
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
            int savedIndex = TeamDisplay::getInstance().m_choiceBox.getChoiceIndex();
            TeamDisplay::getInstance().addSwitchChoice(TeamDisplay::getInstance().m_choiceBox.getChoiceName());
            TeamDisplay::getInstance().m_subChoiceBox.setVisible(false);
            TeamDisplay::getInstance().m_choiceBox.setFocus(true);
            std::vector<Choice> choices;
            for (const auto& pkm : TeamDisplay::getInstance().m_team) {
                choices.emplace_back(pkm.m_surname, "OrderChoice", pkm.m_surname);
            }
            
            size_t teamSize = TeamDisplay::getInstance().m_team.size();

            if (teamSize <= 6) {
                for (size_t i = 0; i < 6 - teamSize; ++i) {
                    choices.emplace_back("---", "EmptySlot", std::monostate());
                }
            }
            TeamDisplay::getInstance().m_choiceBox.init(choices);
            TeamDisplay::getInstance().m_choiceBox.setChoiceIndex(savedIndex);
        } else {
            TeamDisplay::getInstance().addSwitchChoice(choice);
            if (TeamDisplay::getInstance().getSwitchChoice().size() > 1) {
                TeamDisplay::getInstance().switchChoice(false);
            }
        }
    });

    GameEvents::ItemsChoice.subscribe([]() {
        std::vector<Choice> choices = {
            {"Donner", "SelectGiveItem", std::monostate()},
            {"Prendre", "TakeItem", std::monostate()},
            {"Echanger", "SwitchItem", std::monostate()},
            {"Retour", "Cancel", std::monostate()}
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

    GameEvents::GiveItem.subscribe([]() {
        if (Bag::getInstance().m_subChoiceBox2.isVisible()) {
            TeamDisplay::getInstance().open();
            TeamDisplay::getInstance().updateDisplay();
            Bag::getInstance().close();
            TeamDisplay::getInstance().m_choiceBox.setFocus(true);
            std::vector<Choice> choices;
            for (const auto& pkm : TeamDisplay::getInstance().m_team) {
                choices.emplace_back(pkm.m_surname, "GiveItem", std::monostate());
            }
    
            size_t teamSize = TeamDisplay::getInstance().m_team.size();

            if (teamSize <= 6) {
                for (size_t i = 0; i < 6 - teamSize; ++i) {
                    choices.emplace_back("---", "EmptySlot", std::monostate());
                }
            }
            TeamDisplay::getInstance().m_choiceBox.init(choices);
            return;
        }
        std::string selectedItemName = Bag::getInstance().getChoiceBox().getChoiceName().substr(0, Bag::getInstance().getChoiceBox().getChoiceName().size() - 3);
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
                    std::vector<Choice> choices = {
                        {"Oui", "YesGiveItem", std::monostate()},
                        {"Non", "Cancel", std::monostate()}
                    };
                    DialogManager::getInstance().setChoiceBox(choices);
                    DialogManager::getInstance().setChoiceBoxVisible(true);
                    DialogManager::getInstance().getChoiceBox().setPosition({280.f, DialogManager::getInstance().getTop()});
                    Menu::getInstance().close();
                    break;
                }
            }
        }
        Bag::getInstance().lookingForItem = false;
        Bag::getInstance().close();
        Menu::getInstance().close();
        TeamDisplay::getInstance().m_subChoiceBox.setVisible(false);
        TeamDisplay::getInstance().updateDisplay();
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

    GameEvents::Cancel.subscribe([]() {
        TeamDisplay::getInstance().getCurrentChoiceBox().setVisible(false);
        TeamDisplay::getInstance().resetSubChoiceBox();
        if (Bag::getInstance().m_subChoiceBox2.isVisible()) {
            Bag::getInstance().getChoiceBox().setFocus(true);
            Bag::getInstance().m_subChoiceBox2.setFocus(false);
        }
        Bag::getInstance().m_subChoiceBox2.setVisible(false);
        DialogManager::getInstance().setChoiceBoxVisible(false);
        if (Bag::getInstance().isOpen()){
            Bag::getInstance().m_subChoiceBox2.setVisible(false);
            Bag::getInstance().m_subChoiceBox2.setFocus(false);
            Bag::getInstance().getChoiceBox().setFocus(true);        
        }
    });

    GameEvents::SwitchItem.subscribe([]() {
        if (TeamDisplay::getInstance().m_subChoiceBox.isVisible()) {
            std::string pkmSelected = TeamDisplay::getInstance().m_choiceBox.getChoiceName();
            TeamDisplay::getInstance().m_subChoiceBox.setVisible(false);
            TeamDisplay::getInstance().m_choiceBox.setFocus(true);
            std::vector<Choice> choices;
            for (const auto& pkm : TeamDisplay::getInstance().m_team) {
                if (pkm.m_surname == pkmSelected) {
                    if (pkm.m_item.empty()) {
                        DialogManager::getInstance().startDialogue({{pkm.m_surname + " ne tient rien.", BoxType::Classic}});
                        TeamDisplay::getInstance().resetSubChoiceBox();
                        TeamDisplay::getInstance().updateDisplay();
                        return;
                    }
                    TeamDisplay::getInstance().addSwitchChoice(pkm.m_item);
                    TeamDisplay::getInstance().addSwitchChoice(pkm.m_surname);
                }
                choices.emplace_back(pkm.m_surname, "SwitchItem", std::monostate());
            }

            
            size_t teamSize = TeamDisplay::getInstance().m_team.size();

            if (teamSize <= 6) {
                for (size_t i = 0; i < 6 - teamSize; ++i) {
                    choices.emplace_back("---", "EmptySlot", std::monostate());
                }
            }
            TeamDisplay::getInstance().m_choiceBox.init(choices);
        } else {
            std::string firstPkmSelected = TeamDisplay::getInstance().getSwitchChoice().back();
            TeamDisplay::getInstance().getSwitchChoice().pop_back();
            std::string item = TeamDisplay::getInstance().getSwitchChoice().back();
            TeamDisplay::getInstance().getSwitchChoice().pop_back();
            std::string pkmSelected = TeamDisplay::getInstance().m_choiceBox.getChoiceName();
            std::string sndItem = "";
            std::string dialogue = "";


            if (pkmSelected == firstPkmSelected) {                
                TeamDisplay::getInstance().addSwitchChoice(item);
                TeamDisplay::getInstance().addSwitchChoice(pkmSelected);
                return;
            }

            for (auto& pkm : TeamDisplay::getInstance().m_team) {
                if (pkm.m_surname == pkmSelected) {
                    if (!pkm.m_item.empty()) {
                        dialogue = "Vous avez échangé 1 " + item + " de " + firstPkmSelected + " contre 1 " + pkm.m_item + " de " + pkm.m_surname + ".";
                    }
                    else dialogue = pkmSelected + " tient " + item + " !";
                    sndItem = pkm.m_item;
                    pkm.m_item = item;
                }
            }
            for (auto& pkm : TeamDisplay::getInstance().m_team) {
                if (pkm.m_surname == firstPkmSelected)  pkm.m_item = sndItem;
            }
            DialogManager::getInstance().startDialogue({{dialogue, BoxType::Classic}});
            TeamDisplay::getInstance().resetSubChoiceBox();
            TeamDisplay::getInstance().updateDisplay();
        }
    });

    GameEvents::ViewItem.subscribe([]() {
        Bag::getInstance().resetSubChoiceBox();
        Bag::getInstance().m_subChoiceBox2.open();
        Bag::getInstance().getChoiceBox().setFocus(false);
        Bag::getInstance().m_subChoiceBox2.setFocus(true);
    });

    GameEvents::SelectDiscardItem.subscribe([]() {
        std::string selectedItem = Bag::getInstance().getChoiceBox().getChoiceName().substr(0, Bag::getInstance().getChoiceBox().getChoiceName().size() - 3);
        Item item = ItemDatabase::getInstance().getItem(selectedItem);
        std::vector<Choice> choices;
        choices.emplace_back("1", "DiscardItem", -1);
        for (int i = Player::getInstance().getInventory().getQuantity(item); i > 1; i--) {
            choices.emplace_back(std::to_string(i), "DiscardItem", -1);            
            }
        Bag::getInstance().m_subChoiceBox2.init(choices);
        Bag::getInstance().m_subChoiceBox2.setMaxVisibleChoices(1);
        Bag::getInstance().m_subChoiceBox2.hideCursor(true);
        std::string dialogue = "En jeter combien ?";
        DialogManager::getInstance().startDialogue({{dialogue, BoxType::Classic}});
    });

    GameEvents::DiscardItem.subscribe([](int quantity) {
        std::string selectedItem = Bag::getInstance().getChoiceBox().getChoiceName().substr(0, Bag::getInstance().getChoiceBox().getChoiceName().size() - 3);
        Item item = ItemDatabase::getInstance().getItem(selectedItem); 
        if (quantity == -1){
            std::string newquantity = Bag::getInstance().m_subChoiceBox2.getChoiceName();  
            std::vector<Choice> choices;    
            choices = {{"Oui", "DiscardItem", std::stoi(newquantity)}, {"Non", "Cancel"}};
            std::string dialogue = "Voulez-vous jeter " + newquantity + " " + selectedItem + " ? ";        
            Bag::getInstance().m_subChoiceBox2.init(choices);
            Bag::getInstance().m_subChoiceBox2.setMaxVisibleChoices(6);
            Bag::getInstance().m_subChoiceBox2.hideCursor(false);
            DialogManager::getInstance().startDialogue({{dialogue, BoxType::Classic}});
        } else {          
            Player::getInstance().getInventory().removeItem(item, quantity);
            Bag::getInstance().updateDisplay();
            Bag::getInstance().m_subChoiceBox2.setVisible(false);
            Bag::getInstance().getChoiceBox().setFocus(true);
        }
    });

    GameEvents::UseItem.subscribe([]() {
        if (Bag::getInstance().isOpen()) {
            Bag::getInstance().close();
            TeamDisplay::getInstance().open();
            std::vector<Choice> choices;
            for (const auto& pkm : TeamDisplay::getInstance().m_team) {
                choices.emplace_back(pkm.m_surname, "UseItem", std::monostate());
            }
    
            size_t teamSize = TeamDisplay::getInstance().m_team.size();

            if (teamSize <= 6) {
                for (size_t i = 0; i < 6 - teamSize; ++i) {
                    choices.emplace_back("---", "EmptySlot", std::monostate());
                }
            }
            TeamDisplay::getInstance().m_choiceBox.init(choices);
            TeamDisplay::getInstance().m_choiceBox.setFocus(true);
        }
        else {
            std::string selectedItem = Bag::getInstance().getChoiceBox().getChoiceName().substr(0, Bag::getInstance().getChoiceBox().getChoiceName().size() - 3);
            Item item = ItemDatabase::getInstance().getItem(selectedItem);
            std::string selectedPkm = TeamDisplay::getInstance().m_choiceBox.getChoiceName();
            std::string dialogue = "";
            if (Player::getInstance().getInventory().getQuantity(item) > 0){
                dialogue = TeamDisplay::getInstance().heal(item.m_effects, selectedPkm);
                if (dialogue != "Cela n'aura aucune effet...") Player::getInstance().getInventory().removeItem(item, 1);
                int savedIndex = TeamDisplay::getInstance().m_choiceBox.getChoiceIndex();
                std::vector<Choice> choices;
                for (const auto& pkm : TeamDisplay::getInstance().m_team) {
                    choices.emplace_back(pkm.m_surname, "UseItem", std::monostate());
                }
                
                size_t teamSize = TeamDisplay::getInstance().m_team.size();

                if (teamSize <= 6) {
                    for (size_t i = 0; i < 6 - teamSize; ++i) {
                        choices.emplace_back("---", "EmptySlot", std::monostate());
                    }
                }
                TeamDisplay::getInstance().m_choiceBox.init(choices);
                TeamDisplay::getInstance().m_choiceBox.setChoiceIndex(savedIndex);
                Bag::getInstance().updateDisplay();
                if (Player::getInstance().getInventory().getQuantity(item) == 0) dialogue = "Vous n'avez plus de " + selectedItem + " .";
                DialogManager::getInstance().startDialogue({{dialogue, BoxType::Classic}});
            }
            else {
                TeamDisplay::getInstance().m_choiceBox.setFocus(false);
                TeamDisplay::getInstance().m_choiceBox.setVisible(false);
                Bag::getInstance().open();
                Bag::getInstance().getChoiceBox().setFocus(true);
            }
            // TeamDisplay::getInstance().close();
            // Bag::getInstance().open()
        }
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

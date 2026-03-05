#include "TeamDisplay.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "DialogManager.hpp"
#include "EventManager.hpp"
#include "Menu.hpp"
#include "Pokemon.hpp"

TeamDisplay::TeamDisplay() {
    Controller::getInstance().onAxisChanged("MoveHorizontal", [this](float val) {
        // On ne gère l'input que si la boîte de choix est visible (le sac est ouvert)
        if (!m_isOpen) return;

        // Cooldown pour éviter le défilement trop rapide
        if (m_inputClock.getElapsedTime().asSeconds() < 0.2f) return;
        if (std::abs(val) < 0.5f) return;

        if (val < 0 && m_currentpocketIndex > 0) m_currentpocketIndex--;
        else if (val > 0 && m_currentpocketIndex == 0) m_currentpocketIndex++;

        updateDisplay();
        m_inputClock.restart();
    });
    Controller::getInstance().onActionPressed("Cancel", [this]() {
        m_isOpen = false;
        DialogManager::getInstance().setActive(false);
        m_choiceBox.setVisible(false);
        m_choiceBox.reset();
        Menu::getInstance().open();
    });

    GameEvents::OpenPokemon.subscribe([this]() {
        open();
    });

    Controller::getInstance().onActionPressed("OpenMenu", [this]() {
        if (m_isOpen) {
            m_isOpen = false;
            DialogManager::getInstance().setActive(false);
            m_choiceBox.setVisible(false);
            m_choiceBox.reset();
            Menu::getInstance().close();
        }
    });
}

void TeamDisplay::open() {
    m_isOpen = true;
    updateDisplay();
    m_choiceBox.setVisible(true);
    m_choiceBox.setChoiceIndex(0);
}

void TeamDisplay::displayDescription(){
    std::string description;
    std::string description2;
    // On récupère l'item sélectionné
    std::string selectedPkm = m_choiceBox.getChoiceName();


    if (selectedPkm.size() <= 3 || selectedPkm == "Retour") {
        DialogManager::getInstance().setActive(false);
        m_descriptionDialog.hide();
        return;
    }

    if (!selectedPkm.empty()) {
        for (const auto& pkm : m_team) {
            if (pkm.m_surname == selectedPkm) {
                PokemonDataBase& db = PokemonDataBase::getInstance();
                const Pokemon& basePkm = db.getPokemon(selectedPkm);
                if (m_currentpocketIndex == 0) {
                    description = "$[blue]" + pkm.m_nature + "$[black] de nature.                   " + pkm.m_surname + "    " + pkm.m_sexe + "\nRencontré au N. " + std::to_string(pkm.m_encounterLevel) + "                   N." + std::to_string(pkm.m_level) + "\nle 04 mars 2026.\nProvenance :\n$[blue]Renouet.$[black]\n" + pkm.m_description + "\nN° Pokédex : " + std::to_string(basePkm.m_pkdxnumber) + "\nNom : " + basePkm.m_name + "\nType : " + basePkm.m_type + "\nD.O. : $[blue]" + pkm.m_surname + "$[black]\nN° ID : " + std::to_string(pkm.m_id);
                    description2 = "Points Exp. : " + std::to_string(pkm.m_xp) + "                      Objet : " + pkm.m_item + "\nNiveau suivant : " + std::to_string(pkm.toNextLevel(pkm.m_level, pkm.m_xpType));
                }
                else {
                    description = highlightWithNature(pkm);
                    description2 = "CAP SPÉ     " + pkm.m_ability + "                      Objet : " + pkm.m_item + "\nBooste les capacités \nEau en cas de besoin.";
                }
                break;
            }
        }
        std::string texture = "assets/sprite/pokemon/" + selectedPkm + ".png";
        m_bagSprite.setTexture(TextureManager::getInstance().get(texture));
        m_bagSprite.setPosition(235, 50); // Positionner le sprite à un endroit approprié
        m_bagSprite.setScale(3.f, 3.f); // Redimensionner le sprite si nécessaire
    }
    m_descriptionDialog.setText(description, false);
    DialogManager::getInstance().startDialogue({{description2, BoxType::Classic, nullptr, true}}, nullptr, nullptr, 45);
    if (description == "Description manquante") {
        DialogManager::getInstance().setActive(false);
    }
    m_descriptionDialog.show();
}

std::string TeamDisplay::highlightWithNature(const PokemonInstance& pkm) {
    std::vector<int> bonusNature = pkm.m_bonusNature;
    std::string result = "PV     " + std::to_string(pkm.m_currentPV) + " / " + std::to_string(pkm.m_stats[0]) + "                   " + pkm.m_surname + "    " + pkm.m_sexe;
    std::vector<std::string> statNames = {"ATTAQUE", "DÉFENSE", "ATQ SPÉ", "DEF SPÉ", "VITESSE"};
    for (size_t i = 0; i < bonusNature.size(); ++i) {
        if (bonusNature[i] < 0) {
            result += "\n$[blue]" + statNames[i] + "$[black]     " + std::to_string(pkm.m_stats[i]);
        }
        else if (bonusNature[i] > 0) {
            result += "\n$[red]" + statNames[i] + "$[black]     " + std::to_string(pkm.m_stats[i]);
        }
        else {
            result += "\n" + statNames[i] + "     " + std::to_string(pkm.m_stats[i]);
        }
        if (i == 0) {
            result += "                   N." + std::to_string(pkm.m_level);
        }
    }
    return result;
}

void TeamDisplay::updateDisplay() {

    m_pocketDialog.setText("Pokémon", false);
    m_pocketDialog.show();

    std::vector<std::pair<std::string, std::string>> choices;
    
    // On récupère le nom de la poche actuelle pour l'afficher (optionnel, mais utile)
    // Ici on liste juste les items
    for (auto& pkm : m_team) {
        choices.emplace_back(pkm.m_surname, "ViewPkm");
    }
    size_t teamSize = m_team.size();

    if (teamSize <= 6) {
        for (size_t i = 0; i < 6 - teamSize; ++i) {
            choices.emplace_back("---", "EmptySlot");
        }
    }
    m_choiceBox.init(choices);
}

void TeamDisplay::addPokemon(const PokemonInstance& pkm)
{
    m_team.push_back(pkm);
}

void TeamDisplay::draw(sf::RenderWindow& window)
{
    if (!m_isOpen) return;
    
    displayDescription();
    sf::FloatRect choiceBounds = m_choiceBox.getGlobalBounds();
    float height = 50.f;

    m_pocketDialog.setVerticalPadding(10.f);
    m_pocketDialog.setSize({choiceBounds.width, height});
    m_pocketDialog.setPosition({choiceBounds.left, choiceBounds.top - height});

    m_pocketDialog.draw(window);

    m_descriptionDialog.setVerticalPadding(10.f);
    float width = static_cast<float>(window.getSize().x) - choiceBounds.width - 20.f;
    float height2 = static_cast<float>(window.getSize().y)
             - static_cast<float>(DialogManager::getInstance().getHeight());

    m_descriptionDialog.setSize({width, height2});
    m_descriptionDialog.setPosition({0,0});

    m_descriptionDialog.draw(window);

    m_choiceBox.draw(window);

    std::string selectedPkm = m_choiceBox.getChoiceName();
    if (selectedPkm != "---") {
        window.draw(m_bagSprite);
    }
}

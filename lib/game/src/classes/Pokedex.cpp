#include "Pokedex.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "DialogManager.hpp"
#include "EventManager.hpp"
#include "Menu.hpp"
#include "Pokemon.hpp"
#include "Bag.hpp"

Pokedex::Pokedex() {
    std::vector<Choice> choices = {{"Résumé", "SummaryChoice", std::monostate()}, {"Ordre", "OrderChoice", std::string("init")}, {"Objet", "ItemsChoice", std::monostate()}, {"Retour", "Cancel", std::monostate()}};
    m_subChoiceBox.init(choices);
    m_subChoiceBox.setPosition({280.f, m_choiceBox.getPosition().y + m_subChoiceBox.getGlobalBounds().height + 10.f});


    Controller::getInstance().onAxisChanged("MoveHorizontal", [this](float val) {
        // On ne gère l'input que si la boîte de choix est visible (le sac est ouvert)
        if (!m_isOpen || !summary || m_moveChoiceBox.hasFocus()) return;

        // Cooldown pour éviter le défilement trop rapide
        if (m_inputClock.getElapsedTime().asSeconds() < 0.2f) return;
        if (std::abs(val) < 0.5f) return;

        if (summary && val < 0 && m_currentpocketIndex > 0) m_currentpocketIndex--;
        else if (summary && val > 0 && m_currentpocketIndex == 0) m_currentpocketIndex++;

        m_moveChoiceBox.setChoiceIndex(0);
        m_moveChoiceBox.hideCursor(true);

        //updateDisplay();
        m_inputClock.restart();
    });
    Controller::getInstance().onAxisChanged("MoveVertical", [this](float val) {
        if (!m_isOpen) return;

        if (summary && !m_moveChoiceBox.hasFocus() && m_currentpocketIndex == 1) {
            return;
        }
    });
    Controller::getInstance().onActionPressed("SelectMove", [this]() {
        if (!m_isOpen) return;

        if (DialogManager::getInstance().isActive()) return;

        if (summary) {
            m_moveChoiceBox.setFocus(true);
            m_choiceBox.setFocus(false);
            m_moveChoiceBox.hideCursor(false);
        }
    });
    Controller::getInstance().onActionPressed("Cancel", [this]() {
        if (Bag::getInstance().lookingForItem) {
            Bag::getInstance().lookingForItem = false;
            m_choiceBox.setVisible(true);
            m_choiceBox.setFocus(true);
            m_pocketDialog.show();
            Menu::getInstance().close();
            return;
        }
        if (m_moveChoiceBox.hasFocus()) {
            m_moveChoiceBox.setFocus(false);
            m_moveChoiceBox.hideCursor(true);
            m_choiceBox.setFocus(true);
            Menu::getInstance().close();
            return;
        }
        if (summary) {
            summary = false;
            m_currentpocketIndex = 0;
            m_pocketDialog.setText("Pokémon", false);
            m_choiceBox.setFocus(true);
            DialogManager::getInstance().setActive(false);
            Menu::getInstance().close();
            return;
        }
        if (m_subChoiceBox.isVisible()) {
            m_subChoiceBox.setVisible(false);
            m_subChoiceBox.setFocus(false);
            m_choiceBox.setFocus(true);
            resetSubChoiceBox();
            return;
        }
        m_isOpen = false;
        DialogManager::getInstance().setActive(false);
        m_choiceBox.setVisible(false);
        m_choiceBox.reset();
        Menu::getInstance().open();
        Menu::getInstance().setFocus(true);
        summary = false;
    });
}

void Pokedex::open() {
    m_isOpen = true;
    updateDisplay();
    m_choiceBox.setVisible(true);
    m_choiceBox.setChoiceIndex(0);
}

void Pokedex::close() {
    m_isOpen = false;
    summary = false;
    m_currentpocketIndex = 0;

    DialogManager::getInstance().setActive(false);

    m_choiceBox.setVisible(false);
    m_choiceBox.setFocus(true);
    m_choiceBox.reset();

    m_moveChoiceBox.setVisible(false);
    m_moveChoiceBox.setFocus(false);
    m_moveChoiceBox.hideCursor(true);

    m_subChoiceBox.setVisible(false);
    m_subChoiceBox.setFocus(false);
    resetSubChoiceBox();

    m_descriptionDialog.hide();
}

void Pokedex::displayDescription(){
    std::string description;
    std::string description2;
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
                    description = "$[blue]" + pkm.m_nature + "$[black] de nature.                   " + pkm.m_surname + "    " + pkm.m_sexe + "\nRencontré au N. " + std::to_string(pkm.m_encounterLevel) + "                   N." + std::to_string(pkm.m_level)  + "\nle 04 mars 2026." + "                   " + ((pkm.m_statut == "None") ? "" : pkm.m_statut) + "\nProvenance :\n$[blue]Renouet.$[black]\n" + pkm.m_description + "\nN° Pokédex : " + std::to_string(basePkm.m_pkdxnumber) + "\nNom : " + basePkm.m_name + "\nType : " + basePkm.m_type + "\nD.O. : $[blue]" + pkm.m_surname + "$[black]\nN° ID : " + std::to_string(pkm.m_id);
                    description2 = "Points Exp. : " + std::to_string(pkm.m_xp) + "                      Objet : " + pkm.m_item + "\nNiveau suivant : " + std::to_string(pkm.toNextLevel(pkm.m_level, pkm.m_xpType));
                }
                else {
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
    //DialogManager::getInstance().startDialogue({{description2, BoxType::Classic, nullptr, true}}, nullptr, nullptr, 45);
    if (description == "Description manquante") {
        DialogManager::getInstance().setActive(false);
    }

    if (summary) {
        m_descriptionDialog.show();
        DialogManager::getInstance().startDialogue({{description2, BoxType::Classic, nullptr, true}}, nullptr, nullptr, 45);
    }
}

void Pokedex::resetSubChoiceBox() {
    std::vector<Choice> choices = {{"Résumé", "SummaryChoice", std::monostate()}, {"Ordre", "OrderChoice", std::string("init")}, {"Objet", "ItemsChoice", std::monostate()}, {"Retour", "Cancel", std::monostate()}};
    m_subChoiceBox.init(choices);
    m_subChoiceBox.reset();
}

void Pokedex::updateDisplay() {
    int savedIndex = m_choiceBox.getChoiceIndex();
    std::vector<Choice> choices;
    m_pocketDialog.setText("Pokémon", false);

    for (auto& pkm : m_team) {
        choices.emplace_back(pkm.m_surname, "ViewPokemon", std::monostate());
    }
    size_t teamSize = m_team.size();

    if (teamSize <= 6) {
        for (size_t i = 0; i < 6 - teamSize; ++i) {
            choices.emplace_back("---", "EmptySlot", std::monostate());
        }
    }
    m_choiceBox.init(choices);
    m_choiceBox.setChoiceIndex(savedIndex);
    m_pocketDialog.show();
}

void Pokedex::draw(sf::RenderWindow& window)
{
    if (!m_isOpen) return;
    
    displayDescription();
    sf::FloatRect choiceBounds = m_choiceBox.getGlobalBounds();
    float height = 50.f;
    auto& dialog = DialogManager::getInstance();

    m_pocketDialog.setVerticalPadding(10.f);
    m_pocketDialog.setSize({choiceBounds.width, height});
    m_pocketDialog.setPosition({choiceBounds.left, choiceBounds.top - height});
    m_choiceBox.setPosition({m_choiceBox.getPosition().x, dialog.getTop()});

    if (m_currentpocketIndex == 0) {
        m_choiceBox.draw(window);
        m_moveChoiceBox.setFocus(false);
        if (DialogManager::getInstance().isActive() && !summary) {
            m_choiceBox.setFocus(false);
        }
        else {
            m_choiceBox.setFocus(true);
        }
    }
    else {
        m_moveChoiceBox.setVisible(true);

        m_moveChoiceBox.setPosition({
            m_moveChoiceBox.getPosition().x,
            dialog.getTop()
        });
        sf::FloatRect moveBounds = m_moveChoiceBox.getGlobalBounds();

        m_pocketDialog.setPosition({moveBounds.left, moveBounds.top - height});
        m_moveChoiceBox.draw(window);   
    }
    if (m_subChoiceBox.isVisible()) {
        m_subChoiceBox.setFocus(true);
        m_choiceBox.setFocus(false);
        m_subChoiceBox.draw(window);
    }
    m_pocketDialog.draw(window);

    if (summary){
        m_descriptionDialog.setVerticalPadding(10.f);
        float width = static_cast<float>(window.getSize().x) - choiceBounds.width - 20.f;
        float height2 = static_cast<float>(window.getSize().y)
                - static_cast<float>(DialogManager::getInstance().getHeight());

        m_descriptionDialog.setSize({width, height2});
        m_descriptionDialog.setPosition({0,0});

        m_descriptionDialog.draw(window);
        std::string selectedPkm = m_choiceBox.getChoiceName();
        if (selectedPkm != "---") {
            window.draw(m_bagSprite);
        }
        else {
            m_choiceBox.setFocus(true);
            m_moveChoiceBox.setFocus(false);
            m_currentpocketIndex = 0;
            m_pocketDialog.setText("Pokémon", false);
        }
    }
}

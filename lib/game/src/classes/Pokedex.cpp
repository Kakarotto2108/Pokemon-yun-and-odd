#include "Pokedex.hpp"
#include "Controller.hpp"
#include "DialogManager.hpp"
#include "Menu.hpp"
#include "Pokemon.hpp"
#include "ResourceManager.hpp"

#include <algorithm>

Pokedex::Pokedex() {
    Controller::getInstance().onActionPressed("Cancel", [this]() {
        if (!m_isOpen) return;

        close();
        Menu::getInstance().open();
        Menu::getInstance().setFocus(true);
    });
}

void Pokedex::open() {
    m_isOpen = true;
    updateDisplay();
    m_choiceBox.setVisible(true);
    m_choiceBox.setFocus(true);
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
                    description = "$[blue]" + pkm.m_nature + "$[black] de nature.                   " + pkm.m_surname + "    " + pkm.m_sexe + "\nRencontré au N. " + std::to_string(pkm.m_encounterLevel) + "                   N." + std::to_string(pkm.m_level)  + "\nle 04 mars 2026." + "                   " + ((pkm.m_statut == "None") ? "" : pkm.m_statut) + "\nProvenance :\n$[blue]Renouet.$[black]\n" + pkm.m_description + "\nN° Pokédex : " + std::to_string(basePkm.m_pkdxnumber) + "\nNom : " + basePkm.m_name + "\nType : " + basePkm.m_type[0] + (basePkm.m_type.size() > 1 ? " / " + basePkm.m_type[1] : "") + "\nD.O. : $[blue]" + pkm.m_surname + "$[black]\nN° ID : " + std::to_string(pkm.m_id);
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

void Pokedex::updateDisplay() {
    int savedIndex = m_choiceBox.getChoiceIndex();
    std::vector<Choice> choices;
    m_pocketDialog.setText("Pokédex", false);

    std::vector<Pokemon> list = PokemonDataBase::getInstance().getListPokemon();
    std::sort(list.begin(), list.end(),
              [](const Pokemon& a, const Pokemon& b) { return a.m_pkdxnumber < b.m_pkdxnumber; });
    int j = 0;
    for (int i = 0; i < 502; ++i) {
        if (j < static_cast<int>(list.size()) && i == list[j].m_pkdxnumber - 1) {
            choices.emplace_back(list[j].m_name, "ViewDescription", std::monostate());
            j++;
        } else {
            choices.emplace_back("---", "Emptyslot", std::monostate());
        }
    }

    m_choiceBox.init(choices);
    m_choiceBox.setChoiceIndex(savedIndex);
    m_pocketDialog.show();
}

void Pokedex::draw(sf::RenderWindow& window)
{
    if (!m_isOpen) return;

    sf::FloatRect choiceBounds = m_choiceBox.getGlobalBounds();
    float height = 50.f;
    auto& dialog = DialogManager::getInstance();

    m_pocketDialog.setVerticalPadding(10.f);
    m_pocketDialog.setSize({choiceBounds.width, height});
    m_pocketDialog.setPosition({choiceBounds.left, choiceBounds.top - height});
    m_choiceBox.setPosition({m_choiceBox.getPosition().x, dialog.getTop()});

    m_choiceBox.draw(window);
    m_choiceBox.setFocus(!DialogManager::getInstance().isActive());
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
        if (selectedPkm != "---") window.draw(m_bagSprite);
    }
}

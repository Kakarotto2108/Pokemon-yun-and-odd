#include "TeamDisplay.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "GameChoiceBox.hpp"
#include "DialogManager.hpp"
#include "EventManager.hpp"
#include "Menu.hpp"
#include "Pokemon.hpp"

TeamDisplay::TeamDisplay() {
    Controller::getInstance().onActionPressed("Cancel", [this]() {
        m_isOpen = false;
        DialogManager::getInstance().setActive(false);
        GameChoiceBox::getInstance().setVisible(false);
        GameChoiceBox::getInstance().reset();
        Menu::getInstance().open();
    });

    GameEvents::OpenPokemon.subscribe([this]() {
        open();
    });

    Controller::getInstance().onActionPressed("OpenMenu", [this]() {
        if (m_isOpen) {
            m_isOpen = false;
        }
    });
}

void TeamDisplay::open() {
    m_isOpen = true;
    updateDisplay();
    GameChoiceBox::getInstance().setVisible(true);
    GameChoiceBox::getInstance().setChoiceIndex(0);
}

void TeamDisplay::displayDescription(){
    std::string description;
    std::string description2;
    // On récupère l'item sélectionné
    std::string selectedPkm = GameChoiceBox::getInstance().getChoiceName();


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
                description = "$[blue]" + pkm.m_nature + "$[black] de nature.                   " + pkm.m_surname + "    " + pkm.m_sexe + "\nRencontré au N. " + std::to_string(pkm.m_encounterLevel) + "                   N." + std::to_string(pkm.m_level) + "\nle 04 mars 2026.\nProvenance :\n$[blue]Renouet.$[black]\n" + pkm.m_description + "\nN° Pokédex : " + std::to_string(basePkm.m_pkdxnumber) + "\nNom : " + basePkm.m_name + "\nType : " + basePkm.m_type + "\nD.O. : $[blue]" + pkm.m_surname + "$[black]\nN° ID : " + std::to_string(pkm.m_id);
                description2 = "Points Exp. : " + std::to_string(pkm.m_xp) + "                      Objet : " + pkm.m_item + "\nNiveau suivant : " + std::to_string(pkm.toNextLevel(pkm.m_level, pkm.m_xpType));
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

void TeamDisplay::updateDisplay() {

    m_pocketDialog.setText("Pokémon", false);
    m_pocketDialog.show();

    std::vector<std::pair<std::string, std::string>> choices;
    
    // On récupère le nom de la poche actuelle pour l'afficher (optionnel, mais utile)
    // Ici on liste juste les items
    for (auto& pkm : m_team) {
        choices.emplace_back(pkm.m_surname, "ViewPkm");
    }
    for (int i = 0; i < 6 - m_team.size(); ++i) {
        choices.emplace_back("---", "EmptySlot");
    }
    

    GameChoiceBox::getInstance().init(choices);
}

void TeamDisplay::addPokemon(const PokemonInstance& pkm)
{
    m_team.push_back(pkm);
}

void TeamDisplay::draw(sf::RenderWindow& window)
{
    if (!m_isOpen) return;
    
    displayDescription();
    sf::FloatRect choiceBounds = GameChoiceBox::getInstance().getGlobalBounds();
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

    std::string selectedPkm = GameChoiceBox::getInstance().getChoiceName();
    if (selectedPkm != "---") {
        window.draw(m_bagSprite);
    }
}

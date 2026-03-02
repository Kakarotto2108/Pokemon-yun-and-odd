#include "Bag.hpp"
#include "Inventory.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "GameChoiceBox.hpp"
#include "DialogManager.hpp"
#include "EventManager.hpp"
#include "Menu.hpp"

Bag::Bag() {
    Controller::getInstance().onAxisChanged("MoveHorizontal", [this](float val) {
        // On ne gère l'input que si la boîte de choix est visible (le sac est ouvert)
        if (!m_isOpen) return;

        // Cooldown pour éviter le défilement trop rapide
        if (m_inputClock.getElapsedTime().asSeconds() < 0.2f) return;
        if (std::abs(val) < 0.5f) return;

        if (val > 0) m_currentpocketIndex++;
        else m_currentpocketIndex--;

        // Gestion du bouclage (0 à 4 car il y a 5 poches)
        if (m_currentpocketIndex < 0) m_currentpocketIndex = 4;
        else if (m_currentpocketIndex > 4) m_currentpocketIndex = 0;

        updateDisplay();
        GameChoiceBox::getInstance().reset();
        m_inputClock.restart();
    });

    GameEvents::OpenBag.subscribe([this]() {
        open();
    });

    GameEvents::CloseBag.subscribe([this]() {
        m_isOpen = false;
        DialogManager::getInstance().setActive(false);
        GameChoiceBox::getInstance().setVisible(false);
        GameChoiceBox::getInstance().reset();
        Menu::getInstance().open();
    });

    Controller::getInstance().onActionPressed("OpenMenu", [this]() {
        if (m_isOpen) {
            m_isOpen = false;
        }
    });
}

void Bag::open() {
    m_isOpen = true;
    updateDisplay();
    GameChoiceBox::getInstance().setVisible(true);
    GameChoiceBox::getInstance().setChoiceIndex(0);
}

void Bag::displayItemDescription(){
    std::string description;
    // On récupère l'item sélectionné
    std::string selectedItem = GameChoiceBox::getInstance().getChoiceName();


    if (selectedItem.size() <= 3 || selectedItem == "Retour") {
        DialogManager::getInstance().setActive(false);
        return;
    }

    selectedItem = selectedItem.substr(0, selectedItem.size() - 3);
    if (!selectedItem.empty()) {
        description = ItemDatabase::getInstance().getItem(selectedItem).getDescription();
        std::string texture = "assets/sprite/items/" + selectedItem + ".png";
        m_bagSprite.setTexture(TextureManager::getInstance().get(texture));
        m_bagSprite.setPosition(590, 375); // Positionner le sprite à un endroit approprié
        m_bagSprite.setScale(3.f, 3.f); // Redimensionner le sprite si nécessaire
    }
    DialogManager::getInstance().startDialogue({{description, BoxType::Classic, nullptr, true}}, nullptr, nullptr, 45);
    if (description == "Description manquante") {
        DialogManager::getInstance().setActive(false);
    }
}

void Bag::updateDisplay() {
    std::string pocketName;
    switch(m_pockets[m_currentpocketIndex]) {
        case ItemPocket::Items: pocketName = "MÉDICAMENTS"; break;
        case ItemPocket::Balls: pocketName = "BALLS"; break;
        case ItemPocket::KeyItems: pocketName = "OBJETS RARES"; break;
        case ItemPocket::TMsHMs: pocketName = "CT & CS"; break;
        case ItemPocket::Berries: pocketName = "BAIES"; break;
    }

    m_pocketDialog.setText(pocketName, false);
    m_pocketDialog.show();

    std::vector<std::pair<std::string, std::string>> choices;
    
    // On récupère le nom de la poche actuelle pour l'afficher (optionnel, mais utile)
    // Ici on liste juste les items
    for (auto& [pocket, items] : Player::getInstance().getInventory().getPockets()) {
        if (m_pockets[m_currentpocketIndex] == pocket) {
            for (auto& [item, count] : items) {
                std::string itemText = item + " x" + std::to_string(count);

                // TODO: Ajouter un événement "UseItem" ou similaire
                choices.emplace_back(itemText, "UseObj");
            }
        }
    }
    
    choices.emplace_back("Retour", "CloseBag"); // Il faudra gérer l'event CloseBag si tu veux une action explicite

    GameChoiceBox::getInstance().init(choices);
}

void Bag::draw(sf::RenderWindow& window)
{
    if (!m_isOpen) return;
    
    displayItemDescription();
    sf::FloatRect choiceBounds = GameChoiceBox::getInstance().getGlobalBounds();
    float height = 50.f;

    m_pocketDialog.setVerticalPadding(10.f);
    m_pocketDialog.setSize({choiceBounds.width, height});
    m_pocketDialog.setPosition({choiceBounds.left, choiceBounds.top - height});

    m_pocketDialog.draw(window);

    std::string selectedItem = GameChoiceBox::getInstance().getChoiceName();

    if (selectedItem.size() > 3 && selectedItem != "Retour") {
        window.draw(m_bagSprite);
    }
}

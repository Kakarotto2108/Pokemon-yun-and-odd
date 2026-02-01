#include "Bag.hpp"
#include "Inventory.hpp"
#include "Player.hpp"
#include "Controller.hpp"
#include "GameChoiceBox.hpp"

Bag::Bag() {
    Controller::getInstance().onAxisChanged("MoveHorizontal", [this](float val) {
        // On ne gère l'input que si la boîte de choix est visible (le sac est ouvert)
        if (!GameChoiceBox::getInstance().isVisible()) return;
        
        // Cooldown pour éviter le défilement trop rapide
        if (m_inputClock.getElapsedTime().asSeconds() < 0.2f) return;
        if (std::abs(val) < 0.5f) return;

        if (val > 0) m_currentpocketIndex++;
        else m_currentpocketIndex--;

        // Gestion du bouclage (0 à 4 car il y a 5 poches)
        if (m_currentpocketIndex < 0) m_currentpocketIndex = 4;
        else if (m_currentpocketIndex > 4) m_currentpocketIndex = 0;

        updateDisplay();
        m_inputClock.restart();
    });
}

void Bag::open() {
    updateDisplay();
    GameChoiceBox::getInstance().setVisible(true);
    GameChoiceBox::getInstance().setChoiceIndex(0);
}

void Bag::updateDisplay() {
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
    
    // Ajout d'une option pour fermer si la poche est vide ou pour quitter
    if (choices.empty()) {
        choices.emplace_back("Poche vide", "");
    }
    choices.emplace_back("Retour", "CloseBag"); // Il faudra gérer l'event CloseBag si tu veux une action explicite

    GameChoiceBox::getInstance().init(choices);
}

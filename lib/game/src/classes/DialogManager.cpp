#include "DialogManager.hpp"
#include <iostream>

void DialogManager::addLine(const std::string& text, BoxType type) {
    m_queue.push({text, type});
}


void DialogManager::startDialogue(const std::vector<DialogueStep>& steps, WorldEntity* speaker, std::function<void()> actionAfter) {
    if (!m_msgBox) {
        std::cerr << "[DialogManager] Erreur : Pas de MessageBox liée (init non appelé)\n";
        return;
    }

    m_currentSpeaker = speaker;
    
    while(!m_queue.empty()) m_queue.pop();

    for (const auto& step : steps) {
        m_queue.push(step);
    }

    if (!m_queue.empty()) {
        m_active = true;
        next(); 
    }

    m_actionAfter = actionAfter;
}

void DialogManager::next() {
    if (!m_active || !m_msgBox) return;

    if (m_queue.empty()) {
        m_active = false;
        m_msgBox->hide();
        if (m_actionAfter) m_actionAfter();
        m_actionAfter = nullptr;
        return;
    }

    DialogueStep current = m_queue.front();
    m_queue.pop();

    // Si une étape est vide (parce qu'elle a été "supprimée" par un script), on passe à la suivante.
    if (current.text.empty() && !current.action) {
        if (!m_queue.empty()) {
            next(); // On appelle la fonction pour la prochaine étape de la file
            return;
        } else {
            // C'était la dernière étape, on ferme la boîte de dialogue
            m_active = false;
            m_msgBox->hide();
            m_currentSpeaker = nullptr;
            return;
        }
    }

    if (current.action) {
        current.action(); // On exécute l'action liée à cette réplique
    }

    if (current.type == BoxType::Object) {
        current.text = "$[white]" + current.text;
    }

    m_msgBox->setBoxType(current.type);
    m_msgBox->setText(current.text);
    m_msgBox->show();
}

void DialogManager::draw(sf::RenderWindow& window) {
    if (m_active && m_msgBox) {
        m_msgBox->draw(window);        
    }
    GameChoiceBox::getInstance().draw(window);
}
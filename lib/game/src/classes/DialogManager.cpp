#include "DialogManager.hpp"
#include <iostream>

void DialogManager::addLine(const std::string& text, BoxType type) {
    m_queue.push({text, type});
}

void DialogManager::startDialogue(const std::vector<DialogueStep>& steps) {
    if (!m_msgBox) {
        std::cerr << "[DialogManager] Erreur : Pas de MessageBox liée (init non appelé)\n";
        return;
    }

    while(!m_queue.empty()) m_queue.pop();

    for (const auto& step : steps) {
        m_queue.push(step);
    }

    if (!m_queue.empty()) {
        m_active = true;
        next(); 
    }
}

void DialogManager::next() {
    if (!m_active || !m_msgBox) return;

    if (m_queue.empty()) {
        m_active = false;
        m_msgBox->hide();
        return;
    }

    DialogueStep current = m_queue.front();
    m_queue.pop();

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
        GameChoiceBox::getInstance().draw(window);
        m_msgBox->draw(window);
        
    }
}
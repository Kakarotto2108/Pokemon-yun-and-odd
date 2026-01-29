#ifndef DIALOGMANAGER_HPP
#define DIALOGMANAGER_HPP

#include "GameDialog.hpp"
#include "GameChoiceBox.hpp"
#include <queue>
#include <functional>
#include <vector>
#include <string>
#include <memory>

class WorldEntity; // Forward declaration

struct DialogueStep {
    std::string text;
    BoxType type = BoxType::Classic;
    std::function<void()> action = nullptr;
};

class DialogManager {
public:
    // Empêche la copie
    DialogManager(const DialogManager&) = delete;
    DialogManager& operator=(const DialogManager&) = delete;

    static DialogManager& getInstance() {
        static DialogManager instance;
        return instance;
    }

    // Indispensable pour lier la MessageBox au Singleton au lancement du jeu
    void init(GameDialog* msgBox) { m_msgBox = msgBox; }

    void addLine(const std::string& text, BoxType type = BoxType::Classic);
    void startDialogue(const std::vector<DialogueStep>& steps, WorldEntity* speaker, std::function<void()> actionAfter = nullptr);
    void next();

    bool isActive() const { return m_active; }
    void draw(sf::RenderWindow& window);
    WorldEntity* getCurrentSpeaker() const { return m_currentSpeaker; }

private:
    DialogManager() = default; // Constructeur privé

    GameDialog* m_msgBox = nullptr;
    WorldEntity* m_currentSpeaker = nullptr;
    std::queue<DialogueStep> m_queue;
    bool m_active = false;
    std::function<void()> m_actionAfter = nullptr;
};

#endif
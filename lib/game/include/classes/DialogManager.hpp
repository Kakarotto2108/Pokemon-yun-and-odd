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
    bool instant = false;
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
    void startDialogue(const std::vector<DialogueStep>& steps, WorldEntity* speaker = nullptr, std::function<void()> actionAfter = nullptr,  std::size_t maxWidth = 60);
    void next();
    void update(float dt);
    
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }
    void draw(sf::RenderWindow& window);
    WorldEntity* getCurrentSpeaker() const { return m_currentSpeaker; }
    std::size_t getHeight() const { return m_msgBox->getHeight(); }
    GameChoiceBox& getChoiceBox() { return m_choiceBox; }
    void setChoiceBox(std::vector<std::pair<std::string, std::string>> choices) { m_choiceBox.init(choices); }
    void setChoiceBoxVisible(bool visible) { m_choiceBox.setVisible(visible); }
    std::size_t getWidth() const { return m_msgBox->getWidth(); }

private:
    DialogManager() = default; // Constructeur privé

    GameDialog* m_msgBox = nullptr;
    WorldEntity* m_currentSpeaker = nullptr;
    std::queue<DialogueStep> m_queue;
    bool m_active = false;
    std::function<void()> m_actionAfter = nullptr;
    GameChoiceBox m_choiceBox;
    std::vector<DialogueStep> wrapDialogueSteps(const std::vector<DialogueStep>& steps, std::size_t maxWidth);
    std::string wrapText(const std::string& text, std::size_t maxWidth);
};

#endif
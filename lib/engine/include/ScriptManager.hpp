#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "CharacterPath.hpp"
#include "DialogManager.hpp"
#include "GameChoiceBox.hpp"
#include "Player.hpp"
#include "Npc.hpp"
#include "Obj.hpp"
#include "Item.hpp"

inline std::string unescape(const std::string& s) {
    std::string res;
    res.reserve(s.length());
    for (std::size_t i = 0; i < s.length(); ++i) {
        if (s[i] == '\\' && i + 1 < s.length()) {
            switch (s[i + 1]) {
                case 'n':
                    res += '\n';
                    ++i;
                    break;
                case '\\':
                    res += '\\';
                    ++i;
                    break;
                default:
                    res += s[i];
            }
        } else {
            res += s[i];
        }
    }
    return res;
}

inline std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

inline std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, (last - first + 1));
}

class ScriptManager {
public:
    static ScriptManager& getInstance() {
        static ScriptManager inst;
        return inst;
    }

    // Récupère un dialogue. S'il n'est pas en mémoire, il va le chercher.
    const std::vector<DialogueStep>& getDialogue(const std::string& key) {
        if (m_cache.find(key) == m_cache.end()) {
            static std::vector<DialogueStep> empty;
            return empty;
        }  
        return m_cache[key];
    }

    static std::vector<sf::Vector2i> parseDirections(const std::string& str)
    {
        std::size_t pos = 0;
        std::vector<sf::Vector2i> directions;

        while (true)
        {
            std::size_t open = str.find('{', pos);
            if (open == std::string::npos)
                break;

            std::size_t comma = str.find(',', open);
            std::size_t close = str.find('}', comma);

            if (comma == std::string::npos || close == std::string::npos)
                break;

            int x = std::stoi(str.substr(open + 1, comma - open - 1));
            int y = std::stoi(str.substr(comma + 1, close - comma - 1));

            directions.push_back(sf::Vector2i(x, y));

            pos = close + 1;
        }
        return directions;
    }

    void loadDialogues(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Erreur : Impossible d'ouvrir " << path << std::endl;
            return;
        }

        std::string line;
        std::string currentKey = "";
        std::vector<DialogueStep> currentSteps;

        while (std::getline(file, line)) {
            // Nettoyage des espaces/retours chariot invisibles
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty() || line[0] == '#') continue; // Sauter lignes vides et commentaires

            // Détection d'un nouveau bloc : [nom_du_dialogue]
            if (line[0] == '[' && line.back() == ']') {
                // Si on change de bloc, on sauvegarde le précédent s'il n'est pas vide
                if (!currentKey.empty()) {
                    m_cache[currentKey] = currentSteps;
                    currentSteps.clear();
                }
                currentKey = line.substr(1, line.size() - 2);
                continue;
            }

            // Lecture d'une ligne de dialogue : S: Texte | T: Type | A: Action
            if (!currentKey.empty()) {
                DialogueStep step;
                
                // On sépare par le pipe |
                auto parts = split(line, '|');
                for (auto& p : parts) {
                    std::string cleanPart = trim(p);

                    if (cleanPart.find("S:") == 0) {
                        step.text = unescape(trim(cleanPart.substr(2)));
                    } 
                    else if (cleanPart.find("T:") == 0) {
                        std::string type = trim(cleanPart.substr(2)); // Nettoyage ici
                        step.type = (type == "Object") ? BoxType::Object : BoxType::Classic;
                    } 
                    else if (cleanPart.find("A:") == 0) {
                        std::string actionData = trim(cleanPart.substr(2)); // Nettoyage ici
                        std::string capturedKey = currentKey;
                        int stepIndex = currentSteps.size();
                        
                        step.action = [this, actionData, capturedKey, stepIndex]() {
                            auto tokens = split(actionData, ':');
                            if (tokens.empty()) return;

                            // On nettoie aussi chaque token pour être sûr
                            std::string cmd = trim(tokens[0]);

                            if (cmd == "GIVE_ITEM") {
                                if (tokens.size() >= 2) {
                                    std::string itemID = trim(tokens[1]);
                                    int qty = std::stoi(trim(tokens[2]));
                                    
                                    // On retire l'objet de l'inventaire du PNJ qui parle
                                    WorldEntity* speaker = DialogManager::getInstance().getCurrentSpeaker();
                                    Character* speakerChar = dynamic_cast<Character*>(speaker);
                                    if (speakerChar) {
                                        // On crée un objet Item pour l'ajouter à l'inventaire.
                                        Item item(itemID, ItemPocket::Items, "Objet reçu d'un PNJ.");
                                        if (speakerChar->getInventory().getQuantity(item) >= qty) {
                                            speakerChar->RemoveItem(item, qty);
                                            Player::getInstance().getInventory().addItem(item, qty);
                                        }
                                    }
                                    else {
                                            cmd = "CHANGE_DIAG";
                                    }
                                    Obj* obj = dynamic_cast<Obj*>(speaker);
                                    if (obj) {
                                        obj->giveItem();
                                        // On crée un objet Item pour l'ajouter à l'inventaire.
                                        Item item(itemID, ItemPocket::Items, "Objet reçu d'un PNJ.");
                                        Player::getInstance().getInventory().addItem(item, qty);
                                    }
                                    // On "supprime" la ligne pour les prochaines fois en la vidant.
                                    if (m_cache.count(capturedKey)) {
                                        auto& stepsInCache = m_cache.at(capturedKey);
                                        if (stepIndex >= 0 && static_cast<std::size_t>(stepIndex) < stepsInCache.size()) {
                                            stepsInCache[stepIndex].text = "";
                                            stepsInCache[stepIndex].action = nullptr;
                                        }
                                    }
                                }
                            }
                            else if (cmd == "CHOICE") {
                                std::vector<std::pair<std::string, std::string>> choices;

                                // On commence à i=1 car i=0 est la commande "CHOICE"
                                for (size_t i = 1; i < tokens.size(); ++i) {
                                    std::string rawChoice = trim(tokens[i]); // Récupère "{choix1, Evenement1}"

                                    // 1. Enlever les accolades { }
                                    if (!rawChoice.empty() && rawChoice.front() == '{' && rawChoice.back() == '}') {
                                        rawChoice = rawChoice.substr(1, rawChoice.size() - 2);
                                    }

                                    // 2. Split sur la virgule pour séparer le texte de l'événement
                                    auto choicePair = split(rawChoice, ',');

                                    if (choicePair.size() == 2) {
                                        std::string text = trim(choicePair[0]);
                                        std::string event = trim(choicePair[1]);
            
                                        choices.emplace_back(text, event);
                                    }
                                }

                                GameChoiceBox::getInstance().init(choices);
                                GameChoiceBox::getInstance().show();
                            }
                            else if (cmd == "MOVE") {
                                if (tokens.size() < 3){
                                    return;
                                }

                                WorldEntity* speaker = DialogManager::getInstance().getCurrentSpeaker();
                                Character* speakerChar = dynamic_cast<Character*>(speaker);

                                if (!speakerChar)
                                    return;

                                std::string action = trim(tokens[1]);
                                sf::Vector2i pos = speakerChar->getPosition();
                                auto path = std::make_unique<CharacterPath>(
                                    (action == "RANDOM") ? PathType::RANDOM : 
                                    (action == "SIMPLE") ? PathType::SIMPLE : 
                                    (action == "LOOP") ? PathType::LOOP : 
                                    PathType::PINGPONG, 2.f);

                                std::string dirStr = trim(tokens[2]);
                                auto directions = parseDirections(dirStr);
                                if (directions.empty())
                                    return;

                                if (action == "SIMPLE" || action == "PINGPONG") {
                                    path->addDestination(pos, directions[0]);
                                }

                                else if (action == "LOOP") {
                                    path->addDestinationLoop(pos, directions);
                                }
                                
                                speakerChar->setPath(std::move(path));
                                speakerChar->getPath()->start();
                            }
                            if (cmd == "CHANGE_DIAG") {
                                WorldEntity* speaker = DialogManager::getInstance().getCurrentSpeaker();
                                Npc* npc = dynamic_cast<Npc*>(speaker);
                                if (tokens.size() >= 2) {
                                    std::string diagKey = trim(tokens[1]);
                                    npc->setDialogueKey(diagKey);
                                }
                            }
                            else if (cmd == "HEAL") {
                                // ...
                            }
                        };
                    }
                }
                currentSteps.push_back(step);
            }
        }

        // Ne pas oublier de sauvegarder le dernier bloc du fichier
        if (!currentKey.empty()) {
            m_cache[currentKey] = currentSteps;
        }

        file.close();
    }

private:
    std::map<std::string, std::vector<DialogueStep>> m_cache;
};
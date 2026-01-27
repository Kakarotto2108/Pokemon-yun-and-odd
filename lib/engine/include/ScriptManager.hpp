#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "DialogManager.hpp"
#include "GameChoiceBox.hpp"

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
                        step.text = trim(cleanPart.substr(2));
                    } 
                    else if (cleanPart.find("T:") == 0) {
                        std::string type = trim(cleanPart.substr(2)); // Nettoyage ici
                        step.type = (type == "Object") ? BoxType::Object : BoxType::Classic;
                    } 
                    else if (cleanPart.find("A:") == 0) {
                        std::string actionData = trim(cleanPart.substr(2)); // Nettoyage ici
                        
                        step.action = [actionData]() {
                            auto tokens = split(actionData, ':');
                            if (tokens.empty()) return;

                            // On nettoie aussi chaque token pour être sûr
                            std::string cmd = trim(tokens[0]);

                            if (cmd == "GIVE_ITEM") {
                                std::string itemID = trim(tokens[1]);
                                int qty = std::stoi(trim(tokens[2]));
                                std::cout << "Don de " << qty << " " << itemID << std::endl;
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
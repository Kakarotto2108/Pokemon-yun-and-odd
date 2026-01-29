#include "ZoneState.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

static std::string xorEncryptDecrypt(const std::string& input, const std::string& key) {
    std::string output = input;
    size_t keyLen = key.size();
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key[i % keyLen];
    }
    return output;
}

class GameInstance {
public:
    static GameInstance& getInstance() {
        static GameInstance instance;
        return instance;
    }

    ZoneState& getZoneState(int zoneId) { return m_zones[zoneId]; }

    void saveZoneState(int zoneId, const std::vector<std::unique_ptr<WorldEntity>>& entities) {
        ZoneState& state = m_zones[zoneId];
        state.entities.clear();
        for (const auto& entity : entities) {
            state.entities[entity->getName()] = entity->getState();
        }
    }

    // ------------------ SAVE ENCRYPTED ------------------
    void saveToFileEncrypted(const std::string& filename) const {
        nlohmann::json j;
        for (const auto& [zoneId,state] : m_zones) {
            j[std::to_string(zoneId)] = state.toJson();
        }

        std::string plaintext = j.dump();
        std::string ciphertext = xorEncryptDecrypt(plaintext, key);

        std::ofstream file(filename, std::ios::binary | std::ios::trunc);
        if(!file) throw std::runtime_error("Impossible d'ouvrir le fichier pour sauvegarde");
        file.write(ciphertext.data(), ciphertext.size());
    }

    // ------------------ LOAD ENCRYPTED ------------------
    void loadFromFileEncrypted(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if(!file) throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde");

        std::string ciphertext((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string plaintext = xorEncryptDecrypt(ciphertext, key);

        nlohmann::json j = nlohmann::json::parse(plaintext);

        for (auto it = j.begin(); it != j.end(); ++it) {
            int zoneId = std::stoi(it.key());
            m_zones[zoneId] = ZoneState::fromJson(it.value());
        }
    }

private:
    std::string key = "LaLuneEstBelle2108";
    GameInstance() = default;
    std::unordered_map<int, ZoneState> m_zones;
};

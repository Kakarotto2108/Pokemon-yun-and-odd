#include <iostream>
#include "Inventory.hpp"

void Inventory::addItem(const Item& item, int quantity)
{
    m_pockets[item.getPocket()][item.getName()] += quantity;
}

bool Inventory::removeItem(const Item& item, int quantity)
{
    auto& pocket = m_pockets[item.getPocket()];
    auto it = pocket.find(item.getName());

    if (it == pocket.end() || it->second < quantity)
        return false;

    it->second -= quantity;

    if (it->second == 0)
        pocket.erase(it);

    return true;
}

int Inventory::getQuantity(const Item& item) const
{
    // On cherche d'abord si la poche existe
    auto pocketIt = m_pockets.find(item.getPocket());
    if (pocketIt == m_pockets.end()) return 0;

    // Puis on cherche l'item dans cette poche
    auto itemIt = pocketIt->second.find(item.getName());
    return (itemIt != pocketIt->second.end()) ? itemIt->second : 0;
}

nlohmann::json Inventory::toJson() const {
    nlohmann::json j;
    for (auto& [pocket, items] : m_pockets) {
        nlohmann::json pocketJson;
        for (auto& [item, count] : items) {
            pocketJson[item] = count;
        }
        j[static_cast<int>(pocket)] = pocketJson;
    }
    return j;
}

Inventory Inventory::fromJson(const nlohmann::json& j) {
    Inventory inventory;
    for (auto it = j.begin(); it != j.end(); ++it) {
        ItemPocket pocket = static_cast<ItemPocket>(std::stoi(it.key()));
        Pocket items;
        for (auto itemIt = it.value().begin(); itemIt != it.value().end(); ++itemIt) {
            items[itemIt.key()] = itemIt.value();
        }
        inventory.m_pockets[pocket] = items;
    }
    return inventory;
}

void Inventory::debugPrint() const {
    std::cerr << "Inventory:\n";
    for (auto& [pocket, items] : m_pockets) {
        std::string pocketName;
        switch(pocket) {
            case ItemPocket::Items: pocketName = "Items"; break;
            case ItemPocket::Balls: pocketName = "Balls"; break;
            case ItemPocket::KeyItems: pocketName = "Key Items"; break;
            case ItemPocket::TMsHMs: pocketName = "TMs/HMs"; break;
            case ItemPocket::Berries: pocketName = "Berries"; break;
        }

        std::cerr << "Pocket: " << pocketName << "\n";

        if (items.empty()) {
            std::cerr << "(vide)\n";
        } else {
            for (auto& [item, count] : items) {
                std::cerr << "- " << item << " x" << count << "\n";
            }
        }
        std::cerr << "\n";
    }
}

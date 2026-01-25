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

void Inventory::debugPrint() const {
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

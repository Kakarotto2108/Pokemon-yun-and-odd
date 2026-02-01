#pragma once
#include <vector>
#include "Item.hpp"
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <map>
using Pocket = std::unordered_map<std::string, int>;

class Inventory
{
public:
    
    void addItem(const Item& item, int quantity = 1);
    bool removeItem(const Item& item, int quantity = 1);

    int getQuantity(const Item& item) const;
    void debugPrint() const;

    nlohmann::json toJson() const;
    static Inventory fromJson(const nlohmann::json& j);
    std::map<ItemPocket, Pocket>& getPockets() { return m_pockets; }

private:
    std::map<ItemPocket, Pocket> m_pockets;  
};

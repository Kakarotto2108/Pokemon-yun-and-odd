#pragma once
#include <vector>
#include "Item.hpp"
#include <unordered_map>
#include <map>
using Pocket = std::unordered_map<std::string, int>;

class Inventory
{
public:
    
    void addItem(const Item& item, int quantity = 1);
    bool removeItem(const Item& item, int quantity = 1);

    int getQuantity(const Item& item) const;
    void debugPrint() const;
private:
    std::map<ItemPocket, Pocket> m_pockets;  
};

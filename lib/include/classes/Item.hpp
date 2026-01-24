#pragma once
#include <string>

enum class ItemPocket
{
    Items,
    Balls,
    KeyItems,
    TMsHMs,
    Berries
};

class Item
{
public:
    Item(
        const std::string& name,
        ItemPocket pocket,
        const std::string& description,
        bool consumable = false
    );

    const std::string& getName() const;
    const std::string& getDescription() const;
    ItemPocket getPocket() const;

    bool isConsumable() const;

private:
    std::string m_name;
    std::string m_description;
    ItemPocket  m_pocket;
    bool        m_consumable;
};

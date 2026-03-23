#include "Item.hpp"

Item::Item(
    std::string name,
    ItemPocket pocket,
    std::string description,
    bool consumable,
    std::vector<std::pair<std::string, std::string>> effects
)
: m_name(std::move(name))
, m_description(std::move(description))
, m_pocket(pocket)
, m_consumable(consumable)
, m_effects(effects)
{}

const std::string& Item::getName() const
{
    return m_name;
}

const std::string& Item::getDescription() const
{
    return m_description;
}

ItemPocket Item::getPocket() const
{
    return m_pocket;
}

bool Item::isConsumable() const
{
    return m_consumable;
}

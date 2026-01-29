#pragma once
#include <unordered_map>
#include <string>
#include "EntityState.hpp"

struct ZoneState {
    std::unordered_map<std::string, EntityState> entities;
};

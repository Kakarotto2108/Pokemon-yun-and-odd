#pragma once
#include <vector>

class CollisionGrid {
public:
    void load(const std::vector<int>& data, unsigned int width, unsigned int height) {
        m_grid = data;
        m_width = width;
        m_height = height;
    }
    
private:
    std::vector<int> m_grid;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
};
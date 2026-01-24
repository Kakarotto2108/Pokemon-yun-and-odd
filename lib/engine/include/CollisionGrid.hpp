#pragma once
#include <vector>

class CollisionGrid {
public:
    void load(const std::vector<int>& data, unsigned int width, unsigned int height) {
        m_grid = data;
        m_width = width;
        m_height = height;
    }

    // Retourne true si la case est bloquante (ex: ID > 0)
    bool isBlocking(int x, int y) const {
        if (x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height) 
            return true; // Hors map = bloqué
        
        // Supposons que 0 = libre, et tout autre chiffre = obstacle
        return m_grid[x + y * m_width] != 0;
    }

private:
    std::vector<int> m_grid;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
};
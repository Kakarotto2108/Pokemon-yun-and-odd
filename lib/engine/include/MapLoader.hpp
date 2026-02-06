#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class MapLoader {
public:
    static std::vector<int> loadFromFile(const std::string& filepath, unsigned int& width, unsigned int& height) {
        std::vector<int> data;
        std::ifstream file(filepath);
        std::string line;
        height = 0;
        width = 0;

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string value;
            unsigned int currentWidth = 0;
            while (std::getline(ss, value, ',')) { // Format CSV simple
                data.push_back(std::stoi(value));
                currentWidth++;
            }
            if (width == 0) width = currentWidth;
            height++;
        }
        return data;
    }
};
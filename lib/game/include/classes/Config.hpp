#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>

struct GameConfig {
    unsigned int width = 1280;
    unsigned int height = 720;
    std::string title = "Pokemon Yun & Odd";
    std::string playerName = "Kakarot";
};

#endif
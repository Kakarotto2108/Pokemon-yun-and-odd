#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>

struct GameConfig {
    unsigned int width = 760;
    unsigned int height = 480;
    std::string title = "Pokemon Yun & Odd";
    std::string playerName = "Kakarot";
};

#endif
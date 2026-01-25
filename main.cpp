#include "Game.hpp"
#include "Config.hpp"

int main()
{
    GameConfig config;
    Game game(config);
    game.run();
    return 0;
}

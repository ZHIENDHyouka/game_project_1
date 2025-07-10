#include "Game.h"
int main(int, char **)
{
    Game &game = Game::Instance();
    game.init();
    game.run();
    return 0;
}

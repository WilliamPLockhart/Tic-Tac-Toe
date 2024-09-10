#include "back/gamestate.hpp"

static Gamestate *game = nullptr;
int main(int argc, char **args)
{
    // here
    game = new Gamestate;
    game->playGame();
    delete game;
    return 1;
}
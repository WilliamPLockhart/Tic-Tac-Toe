#include <iostream>
#include "back/gamestate.hpp"

static Gamestate *game = nullptr;

int main(int argc, char **args)
{
    game = new Gamestate();
    game->init("Hello SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, false);
    // main loop
    while (game->isRunning())
    {
        game->handleEvents();
        game->Update();
        game->render();
    }
    // end of main loop
    game->cleanGame();
    return 0;
}
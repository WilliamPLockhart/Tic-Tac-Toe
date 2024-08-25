#include <iostream>
#include "back/gamestate.hpp"
void *operator new(size_t size)
{
    std::cout << "allocating " << size << "bytes\n";
    return malloc(size);
}

static Gamestate *game = nullptr;

int main(int argc, char **args)
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    game = new Gamestate();
    game->init("Hello SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Gamestate::windowWidth, Gamestate::windowHeight, false);
    // main loop
    while (game->isRunning())
    {
        frameStart = SDL_GetTicks();
        game->handleEvents();
        game->Update();
        game->render();

        // sets FPS to 60
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // end of main loop
    game->cleanGame();
    return 0;
}
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "button.hpp"
#include "words/hash.hpp"
const int Width = 1280, Height = 720;
int n = SDL_Init(SDL_INIT_EVERYTHING);
SDL_Window *win = SDL_CreateWindow("Hello SDL World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED_MASK, Width, Height, SDL_WINDOW_SHOWN);
// using accelerated uses gpu to drawc graphics
SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

int main(int argc, char *arv[])
{
    bool running = 1;
    // e is a struct that contains all events
    SDL_Event e;
    while (running)
    {

        // if there is an event this event will run
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
            {
                running = 0;
                break;
            }
            }
        }
        // displays the ren
        SDL_RenderPresent(ren);
        // refresh color
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
        // clears canvas
        SDL_RenderClear(ren);
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
    // update
}
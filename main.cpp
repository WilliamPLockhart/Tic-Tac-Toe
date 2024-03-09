#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "button.hpp"
#include "words/hash.hpp"
const int Width = 1280, Height = 720;
int n = SDL_Init(SDL_INIT_EVERYTHING);
SDL_Window *win = SDL_CreateWindow("Hello Scrabble World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED_MASK, Width, Height, SDL_WINDOW_SHOWN);
// using accelerated uses gpu to drawc graphics
SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
SDL_Rect rect = {100, 100, 300, 300};
struct V2d
{
    float x, y;
};
SDL_Texture *tex = IMG_LoadTexture(ren, "assets/Scrabble_Tile_B.jpg");
auto x = SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);

float delta = .001;
float current = 0;
int main(int argc, char *arv[])
{
    bool running = 1;
    // e is a struct that contains all events
    SDL_Event e;
    V2d pos{rect.x, rect.y};
    while (running)
    {
        delta = (SDL_GetTicks() - current) / 1000;
        current = SDL_GetTicks();
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
            case SDL_KEYUP:
            {
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    running = 0;
                    break;
                    break;
                }
            }
            }
        }
        auto key = SDL_GetKeyboardState(NULL);
        if (key[SDL_SCANCODE_UP])
        {
            pos.y -= 100 * delta;
        }
        if (key[SDL_SCANCODE_DOWN])
        {
            pos.y += 100 * delta;
        }
        if (key[SDL_SCANCODE_LEFT])
        {
            pos.x -= 100 * delta;
        }
        if (key[SDL_SCANCODE_RIGHT])
        {
            pos.x += 100 * delta;
        }
        rect.x = pos.x;
        rect.y = pos.y;
        // refresh color
        SDL_SetRenderDrawColor(ren, 10, 10, 100, 255);
        // clears canvas
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, &rect);
        // displays the ren
        SDL_RenderPresent(ren);
    }
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
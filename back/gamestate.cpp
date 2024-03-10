#include "gamestate.hpp"

SDL_Window *Gamestate::win = SDL_CreateWindow("hello scrabble world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
SDL_Renderer *Gamestate::ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

Gamestate::Gamestate()
{
}

Gamestate::~Gamestate()
{
}

void Gamestate::shutdown()
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void Gamestate::enter()
{
}

void Gamestate::update()
{
}

void Gamestate::exit()
{
}

void Menu::enter()
{
}

void Menu::update()
{
    // main loop
    while (1)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
            {
                shutdown();
                break;
            }
            }
        }
        auto key = SDL_GetKeyboardState(NULL);
        if (key[SDL_SCANCODE_ESCAPE])
        {
            shutdown();
        }
        SDL_RenderClear(ren);
        // use null because the bacground should stretch the entire screen
        SDL_RenderCopy(ren, backgroundTexture, NULL, NULL);
        SDL_RenderPresent(ren);
    }
    // end main loop
}

void Menu::exit()
{
}
Menu::Menu()
{
    //../assets/Scrabble_Board.jpg

    backgroundTexture = IMG_LoadTexture(ren, "assets/Scrabble_Board.jpg");
    if (!backgroundTexture)
        std::cout << "error loading background " << SDL_GetError() << std::endl;
}

Menu::~Menu()
{
}
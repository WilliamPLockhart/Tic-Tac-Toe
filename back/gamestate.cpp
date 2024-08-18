#include "gamestate.hpp"

SDL_Window *Gamestate::win = nullptr;
SDL_Renderer *Gamestate::ren = nullptr;
bool Gamestate::running = false;

Gamestate::Gamestate()
{
}

// creates window
void Gamestate::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsytems Initialized!.." << std::endl;
        win = SDL_CreateWindow("Hello SDL", xpos, ypos, width, height, flags);
        if (win)
        {
            std::cout << "Window Created!..." << std::endl;
        }
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (ren)
        {
            std::cout << "Renderer Created!..." << std::endl;
        }
        running = true;
    }
}

void Gamestate::Update()
{
}

void Gamestate::render()
{
    SDL_SetRenderDrawColor(ren, 10, 10, 100, 255);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
}

void Gamestate::cleanGame()
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    running = false;
}

void Gamestate::handleEvents()
{
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_KEYUP:
        {
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                running = false;
                break;
            }
        }
        default:
            break;
        }
    }
}

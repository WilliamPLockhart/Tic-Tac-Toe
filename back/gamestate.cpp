#include "gamestate.hpp"

SDL_Surface *Gamestate::icon = nullptr;
SDL_Window *Gamestate::win = nullptr;
SDL_Renderer *Gamestate::ren = nullptr;
bool Gamestate::running = false;

Gamestate::Gamestate()
{
}

// creates window
void Gamestate::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    // initializes screensize, window, and renderer
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
    // sets icon
    const char *file = "assets/logo.bmp";
    setIcon(file);
    // adds first entity
    entityManager.addEntity(ren, "assets/AmongUSRed.png", {100, 100, 300, 400});
}

void Gamestate::Update()
{
}

void Gamestate::render()
{
    // background
    SDL_SetRenderDrawColor(ren, 10, 10, 100, 255);
    entityManager.renderEntities(ren);
    SDL_RenderPresent(ren);
}

// destroys renderer
void Gamestate::cleanGame()
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    std::cout << "Ending gamestate!..." << std::endl;
    running = false;
}

// takes game inputs
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
            auto key = e.key.keysym.scancode;
            if (key == SDL_SCANCODE_ESCAPE)
            {
                running = false;
                break;
            }
            else if (key == SDL_SCANCODE_P)
            {
                SDL_Rect tempRect = {100, 100, 100, 100};
                const char *fileLocation = "assets/AmongUSEggHat.png";
                entityManager.addEntity(ren, fileLocation, tempRect);
                break;
            }
        }
        case SDL_KEYDOWN:
        {
            auto key = SDL_GetKeyboardState(NULL);
            // allows WASD
            if (key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A] || key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S])
            {
                bool left = key[SDL_SCANCODE_A];
                bool right = key[SDL_SCANCODE_D];
                bool up = key[SDL_SCANCODE_W];
                bool down = key[SDL_SCANCODE_S];
                entityManager.moveEntity(left, right, up, down, entityID);
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            // checks if the user is clicking on the img
            if (e.button.button == SDL_BUTTON_LEFT)
            {

                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Rect *playerRect = entityManager.getNearestRect(mouseX, mouseY, entityID);
                if (playerRect)
                {
                    offsetX = mouseX - playerRect->x;
                    offsetY = mouseY - playerRect->y;
                    dragging = true;
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                dragging = false;
            }
            break;
        }
        case SDL_MOUSEMOTION:
        {
            if (dragging)
            {
                SDL_Rect playerRect = entityManager.getRectByID(entityID);
                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Rect temp = {mouseX - offsetX, mouseY - offsetY, playerRect.w, playerRect.h};
                entityManager.setPlayerRect(temp, entityID);
            }
        }
        default:
            break;
        }
    }
}

// sets the game Icon based on the fileLocation given
void Gamestate::setIcon(const char *fileLocation)
{
    icon = SDL_LoadBMP(fileLocation);
    if (!icon)
    {
        std::cout << "icon failed to load" << std::endl;
    }
    else
    {
        SDL_SetWindowIcon(win, icon);
    }
}
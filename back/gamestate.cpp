#include "gamestate.hpp"

int Gamestate::windowWidth = 1280;
int Gamestate::windowHeight = 720;
SDL_Rect tile;
SDL_Surface *Gamestate::icon = nullptr;
SDL_Window *Gamestate::win = nullptr;
SDL_Renderer *Gamestate::ren = nullptr;
bool Gamestate::running = false;

Gamestate::Gamestate()
{
    int length = 720 / 3;
    tile = {0, 0, length, length};
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

    // audio
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0)
    {
        std::cout << "Failed to initialize SDL2_mixer: " << Mix_GetError() << std::endl;
        running = false;
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "Failed to open audio: " << Mix_GetError() << std::endl;
        running = false;
        return;
    }

    // sets icon
    const char *file = "assets/logo.bmp";
    setIcon(file);
    // adds first entity
    entityManager.addEntity(ren, "assets/AmongUSRed.png", {100, 100, 300, 400});
}

// handles game logic
void Gamestate::Update()
{
}

// renders background and entities
void Gamestate::render()
{
    SDL_SetRenderDrawColor(ren, 10, 10, 100, 255);
    SDL_RenderClear(ren);
    bool color = 0;
    int width = tile.w, height = tile.h;

    for (int i = 0; i < 3; i++)
    {

        for (int y = 0; y < 3; y++)
        {
            tile.x = y * width;
            tile.y = i * height;
            // renders 3 by 3
            if (color)
            {
                SDL_SetRenderDrawColor(ren, 203, 188, 129, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(ren, 94, 89, 71, 255);
            }
            color = !color;
            SDL_RenderFillRect(ren, &tile);
        }
    }
    entityManager.renderEntities(ren);
    SDL_RenderPresent(ren);
}

// destroys renderer
void Gamestate::cleanGame()
{
    Mix_CloseAudio();
    Mix_Quit();
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
                SDL_Rect tempRect = {600, 100, 240, 240};
                const char *fileLocation = "";
                entityManager.addEntity(ren, fileLocation, tempRect, 1);
                break;
            }
        }
        // not needed anymore
        //  case SDL_KEYDOWN:
        //  {
        //      auto key = SDL_GetKeyboardState(NULL);
        //      // allows WASD
        //      if (key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A] || key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S])
        //      {
        //          bool left = key[SDL_SCANCODE_A], right = key[SDL_SCANCODE_D], up = key[SDL_SCANCODE_W], down = key[SDL_SCANCODE_S];
        //          entityManager.moveEntity(left, right, up, down, entityID);
        //      }
        //      break;
        //  }
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
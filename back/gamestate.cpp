#include "gamestate.hpp"

int Gamestate::windowWidth = 1280;
int Gamestate::windowHeight = 720;
SDL_Rect tile;
SDL_Surface *Gamestate::icon = nullptr;
SDL_Window *Gamestate::win = nullptr;
SDL_Renderer *Gamestate::ren = nullptr;
bool Gamestate::running = false;
Entity::turnType Gamestate::board[3][3] = {Entity::turnType::empty};

Gamestate::Gamestate()
{
    int length = windowHeight / 3;
    tile = {0, 0, length, length};
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            board[i][j] = Entity::turnType::empty;
        }
    }
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
    entityManager.setTurn(startRandom());
    entityManager.addEntity(ren, {700, 8, 240, 240});
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
                int newX, newY;
                bool lock = false;
                lock = entityManager.SnapToGrid(mouseX, mouseY, tile, newX, newY, board, ren);
                SDL_Rect playerRect = entityManager.getRectByID(entityID);
                playerRect.x = newX;
                playerRect.y = newY;
                entityManager.setPlayerRect(playerRect, entityID);
                if (lock)
                {
                    entityManager.lockEntities(entityID);
                }
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

bool Gamestate::startRandom()
{
    std::srand(std::time(0));
    int randomNumber = std::rand() % 2;
    return randomNumber;
}
#include "gamestate.hpp"

Entity::turnType Gamestate::gameWon = Entity::turnType::empty;
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
        entityManager.addEntity(ren, restartRect, 1);
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
    Uint32 currentTime = SDL_GetTicks();
    if (playerO == 15)
    {
        r = 100, g = 100, b = 100;
        winStartTime = SDL_GetTicks();
        gameWon = Entity::O;
    }
    else if (playerX == 15)
    {
        r = 100, g = 0, b = 0;
        winStartTime = SDL_GetTicks();
        gameWon = Entity::X;
    }
    if (gameWon != Entity::empty && currentTime - winStartTime > WIN_DISPLAY_TIME)
    {
        newGame();
    }
}

// renders background and entities
void Gamestate::render()
{
    SDL_SetRenderDrawColor(ren, 10, 10, 100, 255);
    SDL_RenderClear(ren);
    bool color = 0;
    int width = tile.w, height = tile.h;

    // renders 3 by 3
    for (int i = 0; i < 3; i++)
    {

        for (int y = 0; y < 3; y++)
        {
            tile.x = y * width;
            tile.y = i * height;

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
    SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
    SDL_Rect fillRect = {0, 0, windowWidth, windowHeight};
    if (gameWon != Entity::empty)
    {
        SDL_RenderFillRect(ren, &fillRect);
    }
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
            // if close window is pressed, end game
        case SDL_QUIT:
            running = false;
            break;

        case SDL_KEYUP:
        { // if esc is pressed, end game
            auto key = e.key.keysym.scancode;
            if (key == SDL_SCANCODE_ESCAPE)
            {
                running = false;
                break;
            }
            else if (key == SDL_SCANCODE_P)
            {
                newGame();
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            // checks if the user is clicking on the img
            if (e.button.button == SDL_BUTTON_LEFT)
            {

                SDL_GetMouseState(&mouseCords.mouseX, &mouseCords.mouseY);
                SDL_Rect *playerRect = entityManager.getNearestRect(mouseCords.mouseX, mouseCords.mouseY, entityID);
                if (playerRect)
                {
                    mouseCords.offsetX = mouseCords.mouseX - playerRect->x;
                    mouseCords.offsetY = mouseCords.mouseY - playerRect->y;
                    dragging = true;

                    if ((playerRect->x == restartRect.x) && (playerRect->y == restartRect.y) && (playerRect->h == restartRect.h) && (playerRect->w == restartRect.w))
                    {
                        dragging = false;
                        newGame();
                        restarted = true;
                    }
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            // places X or O at nearest rectangle
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                // ignore
                if (restarted == true)
                {
                    restarted = false;
                    break;
                }
                dragging = false;
                int newX, newY;
                bool lock = false;
                lock = entityManager.SnapToGrid(mouseCords.mouseX, mouseCords.mouseY, tile, newX, newY, board, ren);
                SDL_Rect playerRect = entityManager.getRectByID(entityID);
                playerRect.x = newX;
                playerRect.y = newY;
                entityManager.setPlayerRect(playerRect, entityID);
                if (lock)
                {
                    entityManager.lockEntities(entityID, playerX, playerO);
                }
            }
            break;
        }
        case SDL_MOUSEMOTION:
        {
            // moves tile with player mouse
            if (dragging)
            {
                SDL_Rect playerRect = entityManager.getRectByID(entityID);
                SDL_GetMouseState(&mouseCords.mouseX, &mouseCords.mouseY);
                SDL_Rect temp = {mouseCords.mouseX - mouseCords.offsetX, mouseCords.mouseY - mouseCords.offsetY, playerRect.w, playerRect.h};
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

// random generates who starts
bool Gamestate::startRandom()
{
    std::srand(std::time(0));
    int randomNumber = std::rand() % 2;
    return randomNumber;
}

void Gamestate::newGame()
{
    std::cout << "game restarted!..." << std::endl;
    // resets values
    entityID = 0;
    playerX = 0;
    playerO = 0;
    gameWon = Entity::empty;
    winStartTime = 0;
    restarted = false;
    dragging = false;

    // sets board to empty
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            board[i][j] = Entity::turnType::empty;
        }
    }
    // resets entityManager
    entityManager.clear();
    entityManager.setTurn(startRandom());
    entityManager.addEntity(ren, {700, 8, 240, 240});
    entityManager.addEntity(ren, restartRect, 1);
}
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
    entityManager.addEntity(ren, "assets/AmongUSRed.png", {100, 100, 300, 400});
}

void Gamestate::Update()
{
}

void Gamestate::render()
{
    // background
    SDL_SetRenderDrawColor(ren, 10, 10, 100, 255);
    SDL_RenderClear(ren);
    entityManager.renderEntities(ren);
    SDL_RenderPresent(ren);
}

void Gamestate::cleanGame()
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    std::cout << "Ending gamestate!..." << std::endl;
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
        case SDL_KEYDOWN:
        {
            auto key = SDL_GetKeyboardState(NULL);

            if (key[SDL_SCANCODE_D])
            {
                entityManager.movePlayerEntity('D');
            }
            else if (key[SDL_SCANCODE_A])
            {
                entityManager.movePlayerEntity('A');
            }
            else if (key[SDL_SCANCODE_W])
            {
                entityManager.movePlayerEntity('W');
            }
            else if (key[SDL_SCANCODE_S])
            {
                entityManager.movePlayerEntity('S');
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            // checks if the user is clicking on the img
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                SDL_Rect playerRect = entityManager.getPlayerRect();
                int playerRectX = playerRect.x;
                int playerRectY = playerRect.y;

                SDL_GetMouseState(&mouseX, &mouseY);
                if (((mouseX >= playerRectX) && (mouseX <= playerRectX + playerRect.w)) && (mouseY >= playerRectY && mouseY <= playerRectY + playerRect.h))
                {
                    offsetX = mouseX - playerRectX;
                    offsetY = mouseY - playerRectY;
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
                SDL_Rect playerRect = entityManager.getPlayerRect();
                SDL_GetMouseState(&mouseX, &mouseY);
                SDL_Rect temp = {playerRect.x, playerRect.y, mouseX - offsetX, mouseY - offsetY};

                entityManager.setPlayerRect(temp);
            }
        }

        default:
            break;
        }
    }
}

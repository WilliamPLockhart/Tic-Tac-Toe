#pragma once
#include "Entity.hpp"
#include <cstdlib>
#include <ctime>

class Gamestate
{
public:
    Gamestate();
    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
    void Update();
    void handleEvents();
    void render();
    void cleanGame();
    bool isRunning() { return running; }
    static SDL_Surface *icon;
    void setIcon(const char *fileLocation);
    static int windowWidth;
    static int windowHeight;
    bool startRandom();
    void newGame();

private:
    struct mouseInfo
    {
        int mouseX, mouseY, offsetY, offsetX;
    };
    static Entity::turnType gameWon;
    static SDL_Window *win;
    static SDL_Renderer *ren;
    static bool running;
    Entity entityManager;

    // time
    Uint32 winStartTime = 0;
    const Uint32 WIN_DISPLAY_TIME = 3000;

    // gameWon
    int r, g, b;
    SDL_Rect fillRect = {0, 0, windowWidth, windowHeight};

    // inputs
    SDL_Event e;
    bool dragging = false;
    mouseInfo mouseCords;
    // player and entities

    bool restarted = false;
    SDL_Rect restartRect = {800, 400, 300, 300};
    int entityID = 0;
    int playerX = 0;
    int playerO = 0;
    static Entity::turnType board[3][3];
};
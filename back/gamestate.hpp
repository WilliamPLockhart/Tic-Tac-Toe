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

private:
    static Entity::turnType board[3][3];
    static SDL_Window *win;
    static SDL_Renderer *ren;
    static bool running;
    Entity entityManager;
    // inputs
    SDL_Event e;
    bool dragging = false;
    int mouseX, mouseY, offsetY, offsetX;
    int entityID = 0;
};
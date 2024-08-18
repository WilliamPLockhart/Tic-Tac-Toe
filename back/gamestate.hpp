#pragma once
#include "global.hpp"

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

private:
    static SDL_Window *win;
    static SDL_Renderer *ren;
    static bool running;
    SDL_Event e;
};
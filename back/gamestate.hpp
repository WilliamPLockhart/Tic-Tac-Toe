#pragma once
#include "global.hpp"

// base class
class Gamestate
{
public:
    // abstract class enforced by pure virtual function(s)
    virtual void enter() = 0;
    virtual void update() = 0;
    virtual void exit() = 0;
    void shutdown();
    static SDL_Window *win;
    static SDL_Renderer *ren;
    SDL_Texture *backgroundTexture;
    SDL_Texture *tileTexture;
    Gamestate();
    virtual ~Gamestate();
};

class Menu : public Gamestate
{
public:
    Menu();
    virtual ~Menu();
    void enter();
    void update();
    void exit();
    void loadBoardTiles();
    int pos(int xORy);
};
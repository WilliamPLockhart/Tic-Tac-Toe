#pragma once
#include "global.hpp"

class Gamestate
{
public:
    Gamestate();
    virtual ~Gamestate();
    void enter();
    void update();
    void loadBoardTiles();
    int pos(int xORy);
    void replaceTiles(int needed);
    void shutdown();

    static SDL_Window *win;
    static SDL_Renderer *ren;
    SDL_Texture *backgroundTexture;
    SDL_Texture *tileTexture;

private:
    bool bagEmpty = false;
    float ratio = 1.0;
    int tileWidth = 51;
    tiles T;
};
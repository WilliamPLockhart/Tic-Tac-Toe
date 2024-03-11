#pragma once
#include "iostream"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <windows.h>

static bool running = 1;
static int windowWidth = 1280;
static int windowHeight = 980;
static int k = SDL_Init(SDL_INIT_EVERYTHING);
static SDL_Surface *icon;

// for loading tiles and board
static SDL_Rect tilePlace;
static SDL_Rect backgroundRect;
static SDL_Rect tileRect;
static int textureWidth, textureHeight;
static int tileWidth = 51;
static int tileHeight = 51;

// startingPos is start of board
// blockDif used to calculate where to place tile
static int startingPos = 60;
static int blockDif = 57;

// will contain each tile and position
struct tileInfo
{
    int xPos;
    int yPos;
    std::string tileType;
};
static std::vector<tileInfo> tilesVector;
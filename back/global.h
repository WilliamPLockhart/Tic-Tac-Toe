#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum playerType
{
    empty = 0,
    X = 11,
    O = 17
};

struct entityInfo
{
    SDL_Rect rectangle;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
    bool locked = false;
    std::string desc = "";
    entityInfo()
        : rectangle{0, 0, 0, 0},
          texture(nullptr, SDL_DestroyTexture)
    {
    }
};
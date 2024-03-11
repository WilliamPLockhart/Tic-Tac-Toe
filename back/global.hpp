#pragma once
#include "iostream"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
static int k = SDL_Init(SDL_INIT_EVERYTHING);
static SDL_Rect backgroundRect;
static int textureWidth, textureHeight;
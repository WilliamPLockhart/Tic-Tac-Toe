#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Entity.hpp"
#include "hash.hpp"

const static int windowWidth = 1280;
const static int windowHeight = 720;
static int k = SDL_Init(SDL_INIT_EVERYTHING);
static SDL_Surface *icon;

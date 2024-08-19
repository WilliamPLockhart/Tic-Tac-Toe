#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>

struct EntityInfo
{
    SDL_Texture *entityTexture;
    SDL_Rect entityRect;
    int id;
};

class Entity
{
public:
    bool addEntity(SDL_Renderer *ren, const char *fileLocation, SDL_Rect rect = {0, 0, 0, 0});
    void moveEntity(char direction, int id = 0);
    void renderEntities(SDL_Renderer *ren);

    SDL_Rect getPlayerRect() { return EntityList.at(0).entityRect; }
    void setPlayerRect(SDL_Rect rect) { EntityList.at(0).entityRect = rect; }

private:
    std::vector<EntityInfo> EntityList;
};
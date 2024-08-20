#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
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
    enum turnType
    {
        X = 1,
        O = 0
    };
    bool addEntity(SDL_Renderer *ren, const char *fileLocation, SDL_Rect rect = {0, 0, 0, 0}, bool turns = 0);
    void moveEntity(bool left, bool right, bool up, bool down, int id = 0);
    void renderEntities(SDL_Renderer *ren);

    SDL_Rect *getNearestRect(int mouseX, int mouseY, int &ID);
    SDL_Rect getRectByID(int ID);
    void setPlayerRect(SDL_Rect rect, int ID);
    turnType getTurn() { return turn; }
    void playAudio(const char *fileLocation);
    

private:
    std::vector<EntityInfo> EntityList;
    turnType turn = X;
};
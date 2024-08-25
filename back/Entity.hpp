#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
struct EntityInfo
{
    std::shared_ptr<SDL_Texture> entityTexture;
    SDL_Rect entityRect;
    int id;
};

class Entity
{
public:
    Entity();
    enum turnType
    {
        empty = 2,
        X = 1,
        O = 0
    };
    void addEntity(SDL_Renderer *ren, SDL_Rect rect = {0, 0, 0, 0}, bool restart = 0);
    void renderEntities(SDL_Renderer *ren);
    SDL_Rect *getNearestRect(int mouseX, int mouseY, int &ID);
    SDL_Rect getRectByID(int ID);
    void setPlayerRect(SDL_Rect rect, int ID);
    turnType getTurn() { return turn; }
    void playAudio(const char *fileLocation);
    bool SnapToGrid(int positionX, int positionY, SDL_Rect tile, int &x, int &y, turnType board[3][3], SDL_Renderer *ren);
    void setTurn(int t);
    void lockEntities(int ID, int &playerX, int &playerO);
    int addPoints();
    void clear();

private:
    std::vector<EntityInfo> lockedEntityList;
    std::vector<EntityInfo> EntityList;
    turnType turn = X;
    int pointBoard[3][3] = {{4, 9, 2}, {3, 5, 7}, {8, 1, 6}};
    int gridX, gridY;
};
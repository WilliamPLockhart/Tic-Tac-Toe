#include "Entity.hpp"

// creates playerText
void Entity::addEntity(SDL_Renderer *ren, SDL_Rect rect)
{
    EntityInfo E;
    SDL_Surface *tempSurface;

    if (turn == X)
    {
        tempSurface = IMG_Load("assets/X.png");
        turn = O;
    }
    else
    {
        tempSurface = IMG_Load("assets/O.png");
        rect.x += 340;
        turn = X;
    }
    if (!tempSurface)
    {
        std::cout << "failure" << std::endl;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tempSurface);
    E.entityTexture = tex;
    E.entityRect = rect;
    E.id = EntityList.size();
    EntityList.push_back(E);
    SDL_FreeSurface(tempSurface);
}

// clears memory and renders each entity
void Entity::renderEntities(SDL_Renderer *ren)
{
    for (auto E : EntityList)
    {
        SDL_RenderCopy(ren, E.entityTexture, NULL, &E.entityRect);
    }
    for (auto E : lockedEntityList)
    {
        SDL_RenderCopy(ren, E.entityTexture, NULL, &E.entityRect);
    }
}

/*
todo:
    change ID method, so that it is quadrant based
*/
SDL_Rect *Entity::getNearestRect(int mouseX, int mouseY, int &ID)
{
    for (auto E : EntityList)
    {
        SDL_Rect &tempRect = E.entityRect;

        int tempRectX = tempRect.x;
        int tempRectY = tempRect.y;

        if (((mouseX >= tempRectX) && (mouseX <= tempRectX + tempRect.w)) && (mouseY >= tempRectY && mouseY <= tempRectY + tempRect.h))
        {
            ID = E.id;
            return &tempRect;
        }
    }
    return nullptr;
}

// returns SDL_Rect by ID
SDL_Rect Entity::getRectByID(int ID)
{
    if (ID >= 0 && ID < EntityList.size())
    {
        return EntityList.at(ID).entityRect;
    }
    return {0, 0, 0, 0};
}

// problem function
void Entity::setPlayerRect(SDL_Rect rect, int ID)
{
    if (EntityList.size() > ID && ID >= 0)
    {
        EntityList.at(ID).entityRect = rect;
    }
}

// plays audio file based on fileLocation
void Entity::playAudio(const char *fileLocation)
{
    auto music = Mix_LoadMUS("assets/placePiece.mp3");
    if (Mix_PlayingMusic() == 0)
    {
        if (Mix_PlayMusic(music, 0) == -1)
        {
            std::cerr << "Error playing music: " << Mix_GetError() << std::endl;
        }
    }
}

// snaps to available location, locks entity from being moved again
bool Entity::SnapToGrid(int positionX, int positionY, SDL_Rect tile, int &x, int &y, turnType board[3][3], SDL_Renderer *ren)
{
    int gridX = positionX / tile.w;
    int gridY = positionY / tile.h;
    int newPositionX = gridX * tile.w;
    int newPositionY = gridY * tile.h;

    // if the index is empty
    if (board[gridX][gridY] == empty)
    {
        playAudio("assets/placePiece.mp3");
        x = newPositionX;
        y = newPositionY;
        board[gridX][gridY] = turn;
        SDL_Rect tempRect = {700, 8, 240, 240};
        addEntity(ren, tempRect);
        return 1;
    }
    else
    {
        x = 870;
        y = 8;
        return 0;
    }
}

// prevents tile from being moved if it has been played
void Entity::lockEntities(int ID)
{
    if (ID >= 0 && ID < EntityList.size())
    {
        lockedEntityList.push_back(EntityList.at(ID));
        EntityList.at(ID).entityRect = {};
        EntityList.at(ID).entityTexture = nullptr;
        EntityList.at(ID).id = -1;
    }
}

// for the first player
void Entity::setTurn(int t)
{
    if (t)
    {
        turn = X;
    }
    else
    {
        turn = O;
    }
}
#include "Entity.hpp"

// creates playerText
void Entity::addEntity(SDL_Renderer *ren, const char *fileLocation, SDL_Rect rect, bool turns)
{
    EntityInfo E;
    SDL_Surface *tempSurface;
    if (turns)
    {
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
    }
    else
    {
        tempSurface = IMG_Load(fileLocation);
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
        playAudio("assets/placePiece.mp3");
    }
}

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
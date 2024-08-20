#include "Entity.hpp"

// creates playerText
bool Entity::addEntity(SDL_Renderer *ren, const char *fileLocation, SDL_Rect rect, bool turns)
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

// moves the playerEntity based on direction
void Entity::moveEntity(bool left, bool right, bool up, bool down, int id)
{
    // avoid out of bounds
    if (id < 0 || id >= EntityList.size())
    {
        std::cout << "moveEntity Out Of Bounds!..." << std::endl;
        return;
    }
    if (right)
    {
        EntityList.at(id).entityRect.x += 10;
    }
    else if (left)
    {
        EntityList.at(id).entityRect.x -= 10;
    }
    else if (up)
    {
        EntityList.at(id).entityRect.y -= 10;
    }
    else if (down)
    {
        EntityList.at(id).entityRect.y += 10;
    }
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
}

// problem function
void Entity::setPlayerRect(SDL_Rect rect, int ID)
{
    if (EntityList.size() > ID && ID >= 0)
        EntityList.at(ID).entityRect = rect;
}
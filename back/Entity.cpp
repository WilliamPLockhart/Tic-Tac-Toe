#include "Entity.hpp"

// creates playerText
bool Entity::addEntity(SDL_Renderer *ren, const char *fileLocation, SDL_Rect rect = {0, 0, 0, 0})
{
    EntityInfo E;
    SDL_Surface *tempSurface = IMG_Load(fileLocation);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tempSurface);
    E.entityTexture = tex;
    E.entityRect = rect;
    E.id = EntityList.size() - 1;
    EntityList.push_back(E);
    SDL_FreeSurface(tempSurface);
    if (EntityList.size() == 1)
    {
        playerRect = rect;
    }
}

// moves the playerEntity based on direction
void Entity::movePlayerEntity(char direction)
{
    if ('D')
    {
        EntityList.at(0).entityRect.x += 10;
    }
    else if ('A')
    {
        EntityList.at(0).entityRect.x -= 10;
    }
    else if ('W')
    {
        EntityList.at(0).entityRect.y -= 10;
    }
    else if ('S')
    {
        EntityList.at(0).entityRect.y += 10;
    }
}

void Entity::renderEntities(SDL_Renderer *ren)
{
    SDL_RenderClear(ren);
    for (auto E : EntityList)
    {
        SDL_RenderCopy(ren, E.entityTexture, NULL, &E.entityRect);
    }
}
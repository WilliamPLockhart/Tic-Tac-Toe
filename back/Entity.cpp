#include "Entity.hpp"

// creates playerText
bool Entity::addEntity(SDL_Renderer *ren, const char *fileLocation, SDL_Rect rect)
{
    EntityInfo E;
    SDL_Surface *tempSurface = IMG_Load(fileLocation);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tempSurface);
    E.entityTexture = tex;
    E.entityRect = rect;
    E.id = EntityList.size() - 1;
    EntityList.push_back(E);
    SDL_FreeSurface(tempSurface);
}

// moves the playerEntity based on direction
void Entity::moveEntity(char direction, int id)
{
    // avoid out of bounds
    if (id < 0 || id >= EntityList.size())
    {
        std::cout << "moveEntity Out Of Bounds!..." << std::endl;
        return;
    }
    if (direction == 'D')
    {
        EntityList.at(id).entityRect.x += 10;
    }
    else if (direction == 'A')
    {
        EntityList.at(id).entityRect.x -= 10;
    }
    else if (direction == 'W')
    {
        EntityList.at(id).entityRect.y -= 10;
    }
    else if (direction == 'S')
    {
        EntityList.at(id).entityRect.y += 10;
    }
}

// clears memory and renders each entity
void Entity::renderEntities(SDL_Renderer *ren)
{
    SDL_RenderClear(ren);
    for (auto E : EntityList)
    {
        SDL_RenderCopy(ren, E.entityTexture, NULL, &E.entityRect);
    }
}

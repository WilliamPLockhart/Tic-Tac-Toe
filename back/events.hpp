#pragma once
#include "global.h"
class Events
{
public:
    enum buttonType
    {
        nothingNew = 0,
        newGame = 1,
        endGame = 2
    };

    Events();
    void handleEvents();
    void setExitFlag(buttonType &exitFlag) { m_gameExit = &exitFlag; }
    void setEntityVect(std::shared_ptr<std::vector<std::shared_ptr<entityInfo>>> entity) { m_entities = entity; }
    int *getPlaceTile(playerType arr[][3], int width);
    bool getDragging() { return m_dragging; }

private:
    std::unique_ptr<SDL_Event> m_event = std::make_unique<SDL_Event>();
    std::shared_ptr<std::vector<std::shared_ptr<entityInfo>>> m_entities;
    buttonType *m_gameExit = nullptr;
    bool m_dragging = false;
    int m_mouseX, m_mouseY;
    int m_offsetX, m_offsetY;
    SDL_Rect *m_rect;
    bool m_placeTile = false;
};
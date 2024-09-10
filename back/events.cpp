#include "events.hpp"

Events::Events()
    : m_event(std::make_unique<SDL_Event>()),
      m_entities(std::make_shared<std::vector<std::shared_ptr<entityInfo>>>())
{
}

// will handle all user events while m_gameRunning
void Events::handleEvents()
{
    while (SDL_PollEvent(m_event.get()))
    {
        switch (m_event->type)
        {
        // if close window if pressed, end game
        case SDL_QUIT:

            if (m_gameExit)
            {
                *m_gameExit = endGame;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
        {
            SDL_Rect tempRect;
            SDL_GetMouseState(&m_mouseX, &m_mouseY);

            // checks if the user is hovering over a rectangle
            for (auto &e : *m_entities)
            {
                tempRect = e->rectangle;
                int w = tempRect.w, h = tempRect.h, x = tempRect.x, y = tempRect.y;
                if ((m_mouseX >= x && m_mouseX <= x + w) && (m_mouseY >= y && m_mouseY <= y + h))
                {
                    // if movable, set the set the pointer address to that of the rectangle that is movable
                    if (e->locked == false)
                    {
                        m_dragging = true;
                        m_rect = &e->rectangle;
                        m_offsetX = m_mouseX - m_rect->x;
                        m_offsetY = m_mouseY - m_rect->y;
                    }
                    else
                    {
                        if (e->desc == "resetButton")
                        {
                            *m_gameExit = newGame;
                        }
                    }
                }
            }
            break;
        }
        case SDL_MOUSEMOTION:
        {
            // if m_rect selected and the user is dragging, set m_rect position equal to where the mouse is
            SDL_GetMouseState(&m_mouseX, &m_mouseY);
            if (m_rect)
            {

                if (m_dragging)
                {
                    m_rect->x = m_mouseX - m_offsetX;
                    m_rect->y = m_mouseY - m_offsetY;
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            if (m_dragging)
            {
                m_placeTile = true;
            }
            else
            {
                m_placeTile = false;
            }
            m_rect = nullptr;
            m_dragging = false;

            break;
        }
        default:
            break;
        }
    }
    return;
}

int *Events::getPlaceTile(playerType arr[][3], int width)
{
    if (m_placeTile)
    {
        m_placeTile = false;
        SDL_GetMouseState(&m_mouseX, &m_mouseY);
        int arrX = m_mouseX / width;
        int arrY = m_mouseY / width;
        if (arr[arrY][arrX] == empty)
        {
            int *result = new int[2]{arrY, arrX};
            return result;
        }
    }
    return nullptr;
}
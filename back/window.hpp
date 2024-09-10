#pragma once
#include "global.h"
class Window
{
public:
    Window(const std::string &title, int width, int height, bool fullscreen);
    bool init();
    bool setIcon(const char *fileLocation);
    void render();
    void setEntityVect(std::shared_ptr<std::vector<std::shared_ptr<entityInfo>>> entity) { m_entities = entity; }
    void cleanWindow();
    void updateSizes();
    void drawBoard();
    void addEntity(std::string fileLocation, SDL_Rect rect, std::string desc = "");
    void winnerScreen(playerType winner);
    void reset();
    // window details
    int windowWidth;
    int windowHeight;
    int windowX;
    int windowY;
    bool running;

    // board squares
    int tileWidth;

private:
    int m_counter = 0;
    bool m_winScreen = false;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_win;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_ren;
    bool m_fullScreen;
    std::string m_title;
    SDL_Surface *m_icon;
    std::shared_ptr<std::vector<std::shared_ptr<entityInfo>>> m_entities;
};

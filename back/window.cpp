#include "window.hpp"
#include <fstream>
Window::Window(const std::string &title, int width, int height, bool fullscreen)
    : m_win(nullptr, SDL_DestroyWindow),
      m_ren(nullptr, SDL_DestroyRenderer),
      m_title(title),
      windowWidth(width),
      windowHeight(height),
      m_fullScreen(fullscreen),
      m_icon(nullptr),
      m_entities(std::make_shared<std::vector<std::shared_ptr<entityInfo>>>())
{

    running = init();
    tileWidth = windowWidth / 4;
    if (tileWidth * 3 > windowHeight)
    {
        int diff = (tileWidth * 3 - windowHeight);
        tileWidth -= diff / 3;
    }
}

bool Window::init()
{
    // defines window, renderer, audio, and icon
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsytems Initialized!.." << std::endl;

        // defines window
        m_win = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, m_fullScreen), SDL_DestroyWindow);
        int x, y;
        SDL_GetWindowPosition(m_win.get(), &x, &y);
        windowY = y, windowX = x;
        if (m_win)
        {
            std::cout << "Window Created!..." << std::endl;
        }
        // defines renderer
        m_ren = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(SDL_CreateRenderer(m_win.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
        if (m_ren)
        {
            std::cout << "Renderer Created!..." << std::endl;
        }
        // returns error if m_ren or m_win did no initialize
        if (!m_ren || !m_win)
        {
            std::cout << "Error!.." << SDL_GetError() << std::endl;
            return false;
        }
    }
    // ensures mixer is working
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0)
    {
        std::cout << "Failed to initialize SDL2_mixer: " << Mix_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "Failed to open audio: " << Mix_GetError() << std::endl;
        return false;
    }

    // sets icon
    const char *file = "assets/logo.bmp";
    if (!setIcon(file))
    {
        return false;
    }
    return true;
}

// sets the icon to the bmp img at fileLocation
bool Window::setIcon(const char *fileLocation)
{
    m_icon = SDL_LoadBMP(fileLocation);
    if (!m_icon)
    {
        std::cout << "icon failed to load!..." << std::endl;
        return false;
    }
    else
    {
        SDL_SetWindowIcon(m_win.get(), m_icon);
        return true;
    }
}

void Window::render()
{
    SDL_Color overlayColor;
    SDL_SetRenderDrawColor(m_ren.get(), 10, 10, 100, 255);
    SDL_RenderClear(m_ren.get());
    drawBoard();
    for (auto e : *m_entities)
    {
        if (e->desc == "winX")
        {
            overlayColor = {255, 0, 0, 128};
            if (!m_winScreen)
            {
                m_winScreen = true;
                m_counter = SDL_GetTicks();
            }

            continue;
        }
        else if (e->desc == "winO")
        {
            overlayColor = {0, 0, 0, 128};
            if (!m_winScreen)
            {
                m_winScreen = true;
                m_counter = SDL_GetTicks();
            }

            continue;
        }
        else
        {
            SDL_RenderCopy(m_ren.get(), e->texture.get(), NULL, &e->rectangle);
        }
    }
    if (m_winScreen)
    {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - m_counter < 5000) // Check if 5 seconds have passed
        {
            SDL_SetRenderDrawColor(m_ren.get(), overlayColor.r, overlayColor.g, overlayColor.b, overlayColor.a);
            SDL_SetRenderDrawBlendMode(m_ren.get(), SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(m_ren.get(), NULL);
        }
    }
    SDL_RenderPresent(m_ren.get());
}

void Window::cleanWindow()
{

    Mix_CloseAudio();
    Mix_Quit();
}

void Window::drawBoard()
{
    bool color = 0;
    SDL_Rect rect;
    rect.x = 0, rect.y = 0;
    rect.w = tileWidth, rect.h = tileWidth;
    // renders 3 by 3 board
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < 3; y++)
        {
            // alternates color
            if (color)
            {
                SDL_SetRenderDrawColor(m_ren.get(), 203, 188, 129, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(m_ren.get(), 94, 89, 71, 255);
            }
            color = !color;
            SDL_RenderFillRect(m_ren.get(), &rect);
            rect.x += tileWidth;
        }
        rect.x = 0;
        rect.y += tileWidth;
    }
}

void Window::addEntity(std::string fileLocation, SDL_Rect rect, std::string desc)
{

    // Load the surface from the file
    std::shared_ptr<SDL_Surface> loadedSurface(IMG_Load(fileLocation.c_str()), SDL_FreeSurface);
    if (!loadedSurface)
    {
        std::cout << "Failed to load image: " << SDL_GetError() << std::endl;
        return;
    }

    // Create a 32-bit surface with an alpha channel (RGBA) for transparency
    std::shared_ptr<SDL_Surface> scaledSurface(SDL_CreateRGBSurfaceWithFormat(0, tileWidth, tileWidth, 32, SDL_PIXELFORMAT_RGBA8888), SDL_FreeSurface);
    if (!scaledSurface)
    {
        std::cout << "Failed to create scaled surface: " << SDL_GetError() << std::endl;
        return;
    }

    // Enable blending mode to preserve transparency
    SDL_SetSurfaceBlendMode(scaledSurface.get(), SDL_BLENDMODE_BLEND);

    // Blit (copy and scale) the original surface onto the scaled surface
    if (SDL_BlitScaled(loadedSurface.get(), NULL, scaledSurface.get(), NULL) != 0)
    {
        std::cout << "Failed to scale surface: " << SDL_GetError() << std::endl;
        return;
    }

    // Create a texture from the scaled surface
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> tex(
        SDL_CreateTextureFromSurface(m_ren.get(), scaledSurface.get()), SDL_DestroyTexture);

    if (!tex)
    {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }

    // Add entity to m_entities
    std::shared_ptr<entityInfo> e = std::make_shared<entityInfo>();
    if (desc == "resetButton")
    {
        e->locked = true;
    }
    e->desc = desc;
    e->rectangle = rect;
    e->texture = std::move(tex);
    m_entities->push_back(e);
}

void Window::reset()
{
    m_winScreen = false;
    m_counter = 0;
}

void Window::winnerScreen(playerType winner)
{
    if (m_winScreen)
    {
        return;
    }
    SDL_Rect rect = {windowX,
                     windowY,
                     windowWidth,
                     windowHeight};
    std::shared_ptr<entityInfo> x = std::make_shared<entityInfo>();
    if (winner == X)
    {
        x->desc = "winX";
    }
    else if (winner == O)
    {
        x->desc = "winO";
    }
    x->rectangle = rect;
    x->locked = true;
    x->texture = nullptr;
    m_entities->push_back(x);
}
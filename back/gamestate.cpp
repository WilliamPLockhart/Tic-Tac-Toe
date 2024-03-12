#include "gamestate.hpp"

SDL_Window *Gamestate::win = SDL_CreateWindow("hello scrabble world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
SDL_Renderer *Gamestate::ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

Gamestate::Gamestate()
{

    icon = SDL_LoadBMP("assets/Icon_scrabble.bmp");
    SDL_SetWindowIcon(win, icon);
    // board
    backgroundTexture = IMG_LoadTexture(ren, "assets/Scrabble_Board.jpg");
    if (!backgroundTexture)
        std::cout << "error loading background " << SDL_GetError() << std::endl;
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &textureWidth, &textureHeight);
    // this assumes window height is always smaller than window width.
    if (textureHeight > windowHeight)
    {
        textureHeight = windowHeight;
        textureWidth = windowHeight;
        ratio = windowHeight / 980.0;
        tileWidth *= ratio;
    }
    // loads for Gamestate
    replaceTiles(15);

    // this is for the board
    backgroundRect = {0, 0, textureWidth, textureHeight};
    // this is the green place for new tiles
    tilePlace = {textureWidth, 0, windowWidth - textureWidth, textureHeight};
}

Gamestate::~Gamestate()
{
}

void Gamestate::shutdown()
{
    running = 0;
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void Gamestate::update()
{
    // main loop
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
            {
                shutdown();
                break;
            }
            case SDL_WINDOWEVENT:
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    /* to do*/
                }
            }
            }
        }
        auto key = SDL_GetKeyboardState(NULL);
        if (SDL_KEYDOWN)
        {
            if (key[SDL_SCANCODE_ESCAPE])
            {
                shutdown();
            }
        }

        SDL_RenderClear(ren);
        // use null to so that the bacground should stretch the entire screen is an option
        SDL_RenderCopy(ren, backgroundTexture, NULL, &backgroundRect);
        // draws green border
        SDL_SetRenderDrawColor(ren, 42, 140, 68, 255);
        SDL_RenderFillRect(ren, &tilePlace);
        // draws tiles
        loadBoardTiles();
        SDL_RenderPresent(ren);

        // 60fps
        SDL_Delay(60 / 1000);
    }
    // end main loop
}

void Gamestate::loadBoardTiles()
{
    // loading each tile that is already placed
    tileInfo *T = new tileInfo;
    for (int i = 0; i < tilesVector.size(); i++)
    {
        *T = tilesVector.at(i);
        tileTexture = IMG_LoadTexture(ren, T->tileType.c_str());
        // error message if tile cant render while game is runnig
        if (!tileTexture && running == 1)
        {
            std::cout << "error loading tileTexture " << SDL_GetError() << std::endl;
            tilesVector.erase(tilesVector.begin() + i);
        }

        tileRect = {T->xPos, T->yPos, tileWidth, tileWidth};
        SDL_RenderCopy(ren, tileTexture, NULL, &tileRect);
        // free memory up
        SDL_DestroyTexture(tileTexture);
    }
    delete T;
}

// will return the value for where the tile should be placed
int Gamestate::pos(int xORy)
{
    int offset = 1;
    if (xORy > 7)
        offset++;
    if (xORy > 13)
        offset++;
    int value = blockDif * xORy + (xORy - offset);
    if (value > 0)
    {
        return (startingPos + value) * ratio;
    }
    return startingPos * ratio;
}

void Gamestate::replaceTiles(int needed)
{
    // tiles
    std::string letters = T.getTiles(15);
    if (needed != letters.size())
    {
        bagEmpty = true;
    }
    for (int i = 0; i < needed; i++)
    {
        std::string location = "assets/Scrabble_Tile_";
        if (letters[i] == '.')
        {

            location += "Blank";
        }
        else
        {
            location += letters[i];
        }
        location += ".jpg";
        tilesVector.push_back({pos(i), pos(i), location});
    }
}
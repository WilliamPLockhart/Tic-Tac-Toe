#include "gamestate.hpp"

SDL_Window *Gamestate::win = SDL_CreateWindow("hello scrabble world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
SDL_Renderer *Gamestate::ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

Gamestate::Gamestate()
{
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

void Gamestate::enter()
{
}

void Gamestate::update()
{
}

void Gamestate::exit()
{
}

void Menu::enter()
{
}

void Menu::update()
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
        SDL_SetRenderDrawColor(ren, 42, 140, 68, 255);
        SDL_RenderFillRect(ren, &tilePlace);
        loadBoardTiles();
        SDL_RenderPresent(ren);
    }
    // end main loop
}

void Menu::exit()
{
}

void Menu::loadBoardTiles()
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

        tileRect = {T->xPos, T->yPos, tileWidth, tileHeight};
        SDL_RenderCopy(ren, tileTexture, NULL, &tileRect);

        // free memory up
        SDL_DestroyTexture(tileTexture);
    }
    delete T;
}

Menu::Menu()
{
    icon = SDL_LoadBMP("assets/Icon_scrabble.bmp");
    SDL_SetWindowIcon(win, icon);
    // board
    backgroundTexture = IMG_LoadTexture(ren, "assets/Scrabble_Board.jpg");
    if (!backgroundTexture)
        std::cout << "error loading background " << SDL_GetError() << std::endl;
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &textureWidth, &textureHeight);
    backgroundRect = {0, 0, textureWidth, textureHeight};

    tilePlace = {textureWidth, 0, windowWidth - textureWidth, textureHeight};
    // tiles
    tilesVector.push_back({pos(0), pos(0), "assets/Scrabble_Tile_Blank.jpg"});
    tilesVector.push_back({pos(1), pos(1), "assets/Scrabble_Tile_Blank.jpg"});
    tilesVector.push_back({pos(2), pos(2), "assets/Scrabble_Tile_S.jpg"});
    tilesVector.push_back({pos(3), pos(3), "assets/Scrabble_Tile_C.jpg"});
    tilesVector.push_back({pos(4), pos(4), "assets/Scrabble_Tile_R.jpg"});
    tilesVector.push_back({pos(5), pos(5), "assets/Scrabble_Tile_A.jpg"});
    tilesVector.push_back({pos(6), pos(6), "assets/Scrabble_Tile_B.jpg"});
    tilesVector.push_back({pos(7), pos(7), "assets/Scrabble_Tile_B.jpg"});
    tilesVector.push_back({pos(8), pos(8), "assets/Scrabble_Tile_L.jpg"});
    tilesVector.push_back({pos(9), pos(9), "assets/Scrabble_Tile_E.jpg"});
    tilesVector.push_back({pos(10), pos(10), "assets/Scrabble_Tile_Blank.jpg"});
    tilesVector.push_back({pos(11), pos(11), "assets/Scrabble_Tile_Blank.jpg"});
    tilesVector.push_back({pos(12), pos(12), "assets/Scrabble_Tile_Blank.jpg"});
    tilesVector.push_back({pos(13), pos(13), "assets/Scrabble_Tile_Blank.jpg"});
    tilesVector.push_back({pos(14), pos(14), "assets/Scrabble_Tile_Blank.jpg"});
}

Menu::~Menu()
{
}

// will return the value for where the tile should be placed
int Menu::pos(int xORy)
{
    int offset = 1;
    if (xORy > 7)
        offset++;
    if (xORy > 13)
        offset++;
    int value = blockDif * xORy + (xORy - offset);
    if (value > 0)
    {
        return startingPos + value;
    }
    return startingPos;
}
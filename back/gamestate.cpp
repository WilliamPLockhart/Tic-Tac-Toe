#include "gamestate.hpp"
#include <chrono>
#include <thread>
#include <random>

Gamestate::Gamestate()
    : m_winOBJ(std::make_unique<Window>("Hello SDL", 1920, 1080, 0)),
      m_eventOBJ(std::make_unique<Events>()),
      m_entities(std::make_shared<std::vector<std::shared_ptr<entityInfo>>>())
{
    m_gameRunning = m_winOBJ->running;
    setExitFlag(m_exitFlag);
    m_winOBJ->setEntityVect(m_entities);
    m_eventOBJ->setEntityVect(m_entities);
    m_exitFlag = Events::buttonType::newGame;
}

void Gamestate::playGame()
{
    int fps = 60;
    int desiredDelta = 1000 / fps;
    while (m_gameRunning)
    {
        int startLoop = SDL_GetTicks();
        m_eventOBJ->handleEvents();
        update();
        if (m_gameRunning == false)
        {
            break;
        }
        m_winOBJ->render();

        // caps fps
        int delta = SDL_GetTicks() - startLoop;
        if (delta < desiredDelta)
        {
            SDL_Delay(desiredDelta - delta);
        }
    }
    return;
}

void Gamestate::update()
{

    if (m_exitFlag == Events::buttonType::endGame)
    {
        cleanGame();
    }
    else if (m_exitFlag == Events::buttonType::newGame)
    {
        newGame();
    }
    place();
    checkWon();
    if (m_winner)
    {

        m_winOBJ->winnerScreen(m_winner);
    }
    return;
}

void Gamestate::cleanGame()
{

    m_winOBJ->cleanWindow();
    SDL_Quit();
    std::cout << "Ending gamestate!..." << std::endl;
    m_gameRunning = false;
}

void Gamestate::newGame()
{
    std::cout << "Creating new game!..." << std::endl;
    m_winner = empty;
    m_exitFlag = Events::buttonType::nothingNew;
    // set board to empty
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m_board[i][j] = playerType::empty;
        }
    }
    // removes any pre-existing entities
    m_entities->clear();
    m_winOBJ->reset();
    // adds reset button
    SDL_Rect game_button = {m_winOBJ->tileWidth * 3, m_winOBJ->tileWidth * 2, m_winOBJ->tileWidth, m_winOBJ->tileWidth};
    m_winOBJ->addEntity("assets/play-button.png", game_button, "resetButton");

    // determines player turn
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    std::string first = "assets/";
    std::string middle;
    std::string last = ".png";
    if (dis(gen) == 1)
    {
        m_playerMove = playerType::X;
        middle = "X";
    }
    else
    {
        m_playerMove = playerType::O;
        middle = "O";
    }

    SDL_Rect tempRect = {m_winOBJ->tileWidth * 3, 0, m_winOBJ->tileWidth, m_winOBJ->tileWidth};
    m_winOBJ->addEntity(first += middle += last, tempRect, middle);
}

void Gamestate::place()
{
    if (m_winner)
        return;
    int *temp = m_eventOBJ->getPlaceTile(m_board, m_winOBJ->tileWidth);
    if (temp)
    {
        for (auto e : *m_entities)
        {
            if (e->locked == false)
            {
                if (temp[0] > 2 || temp[0] < 0 || temp[1] > 2 || temp[1] < 0)
                {
                    e->rectangle.x = m_winOBJ->tileWidth * 3;
                    e->rectangle.y = 0;
                }
                else
                {
                    SDL_Rect rect = e->rectangle;
                    std::string d = e->desc;
                    if (d == "X")
                    {
                        m_board[temp[0]][temp[1]] = X;
                        if (m_entities->size() < 10)
                        {
                            SDL_Rect tempRect = {m_winOBJ->tileWidth * 3, 0, m_winOBJ->tileWidth, m_winOBJ->tileWidth};
                            m_winOBJ->addEntity("assets/O.png", tempRect, "O");
                        }
                    }
                    else
                    {
                        m_board[temp[0]][temp[1]] = O;
                        if (m_entities->size() < 10)
                        {
                            SDL_Rect tempRect = {m_winOBJ->tileWidth * 3, 0, m_winOBJ->tileWidth, m_winOBJ->tileWidth};
                            m_winOBJ->addEntity("assets/X.png", tempRect, "X");
                        }
                    }
                    e->rectangle.x = m_winOBJ->tileWidth * temp[1];
                    e->rectangle.y = m_winOBJ->tileWidth * temp[0];
                    e->locked = true;
                }
                break;
            }
        }
        delete[] temp;
    }
    else
    {
        if (!m_eventOBJ->getDragging())
        {
            for (auto e : *m_entities)
            {
                if (e->locked == false)
                {
                    e->rectangle.x = m_winOBJ->tileWidth * 3;
                    e->rectangle.y = 0;
                    break;
                }
            }
        }
    }
    return;
}

void Gamestate::checkWon()
{
    // checks horizontal
    for (int i = 0; i < 3; i++)
    {
        int horizontal = (m_board[i][0] + m_board[i][1] + m_board[i][2]);
        int vertical = (m_board[0][i] + m_board[1][i] + m_board[2][i]);
        if (horizontal == X * 3)
        {
            m_winner = X;
            return;
        }
        if (horizontal == O * 3)
        {
            m_winner = O;
            return;
        }
        if (vertical == X * 3)
        {
            m_winner = X;
            return;
        }
        if (vertical == O * 3)
        {
            m_winner = O;
            return;
        }
    }

    int diagL = m_board[0][0] + m_board[1][1] + m_board[2][2];
    int diagR = m_board[2][0] + m_board[1][1] + m_board[0][2];
    if (diagL == O * 3)
    {
        m_winner = O;
        return;
    }
    if (diagL == X * 3)
    {
        m_winner = X;
        return;
    }
    if (diagR == O * 3)
    {
        m_winner = O;
        return;
    }
    if (diagR == X * 3)
    {
        m_winner = X;
        return;
    }
}
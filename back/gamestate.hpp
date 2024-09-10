#pragma once
#include "events.hpp"
#include "window.hpp"
class Gamestate
{
public:
    Gamestate();
    bool getRunning() { return m_gameRunning; }
    void render();
    void playGame();
    void update();
    void cleanGame();
    void setExitFlag(Events::buttonType &exitFlag) { m_eventOBJ->setExitFlag(exitFlag); }
    void newGame();
    void place();
    void checkWon();

private:
    playerType m_board[3][3];
    std::unique_ptr<Window> m_winOBJ;
    std::unique_ptr<Events> m_eventOBJ;
    playerType m_winner = empty;
    bool m_gameRunning;
    Events::buttonType m_exitFlag = Events::buttonType::nothingNew;
    std::shared_ptr<std::vector<std::shared_ptr<entityInfo>>> m_entities;
    playerType m_playerMove;
};
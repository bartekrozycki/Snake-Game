#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <deque>
#include <stdexcept>
#include "winsys.h"
#include "cpoint.h"

class Snake {
    deque<CPoint> food;
    deque<CPoint> body;
    CPoint spawnPoint;
    CRect border;
    char c_head;
    char c_body;
public:
    size_t score;
    enum class Direction{
        RIGHT, LEFT, DOWN, UP
    } dir, last_dir;


    Snake(CRect _border, CPoint spawnPoint = CPoint(5,5), char char_head = '@', char char_body = '+');
    void setBorder(CRect &rect);
    void init(void);
    void changeDirection(Direction snake_dir);
    void paint();
    void spawnFood();
    size_t getLevel() const;
    bool move();
    bool checkCrash(const CPoint &head) const;
    void ai();
    CPoint nextCell(Direction d);
};
class CSnake : public CFramedWindow
{
    enum class GameMode {
        PLAY_MODE,
        HELP_MODE,
        PAUSE_MODE,
        GAME_OVER,
        AI_MODE,

    } gameMode;
    Snake player;
    void paintHelp(void);
    void paintGameOver(void);
public:
    CSnake(CRect r, char _c = ' ');
    bool handleEvent(int key) override;
    void paint() override;
};
#endif

#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <vector>
#include <assert.h>
#include <stdexcept>
#include "winsys.h"
#include "cpoint.h"

class Snake {
    vector<CPoint> body;
    CPoint spawnPoint;
    CRect border;
    char c_head;
    char c_body;
public:
    size_t score;
    size_t level;
    enum class Direction{
        RIGHT, LEFT, DOWN, UP
    } dir, last_dir;


    Snake(CRect _border, CPoint spawnPoint = CPoint(5,5), char char_head = '@', char char_body = '+');
    void setBorder(CRect &rect);
    void init(void);
    void changeDirection(Direction dir);
    void paint();
    void move();
};
class CSnake : public CFramedWindow
{
    enum class GameMode {
        PLAY_MODE,
        HELP_MODE,
        PAUSE_MODE,
    } gameMode;
    Snake player;
    void paintHelp(void);
public:
    CSnake(CRect r, char _c = ' ');
    bool handleEvent(int key) override;
    void paint() override;
};
#endif

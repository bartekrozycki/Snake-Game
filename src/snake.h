#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <vector>
#include <assert.h>
#include <stdexcept>
#include "winsys.h"
#include "cpoint.h"

enum CellType {
    EMPTY,
    FOOD,
    SNAKE_NODE,
};
class Cell {
    size_t x;
    size_t y;
    CellType cellType;
public:
    Cell(size_t _x, size_t _y);
    CellType getCellType();
    void setCellType(CellType _cellType);
    size_t getX();
    size_t getY();
    void set(size_t x, size_t y);
    bool operator==(const Cell& cell);
};

class Board {
    const size_t x;
    const size_t y;
    Cell **cells;
public:
    Board(size_t window_x, size_t window_y);
    ~Board();
    Cell** getCells();
    Cell& getCell(size_t x, size_t y);
    void generateFood();
};

class Snake {
    vector<Cell> snakeParts;
    Cell head;
public:
    Snake(Cell &initPos);
    void grow();
    void move(Cell& nextCell);
    bool checkCrash(Cell& nextCell);
    vector<Cell>& getBodyParts();
};

class CSnake : public CFramedWindow
{
    enum Direction {
        DIRECTION_NONE,
        DIRECTION_LEFT,
        DIRECTION_RIGHT,
        DIRECTION_UP,
        DIRECTION_DOWN,
    } direction;
public:
    CSnake(CRect r, char _c = ' ');
    bool handleEvent(int key) override;
    void paint() override;
};
#endif

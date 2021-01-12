#include "snake.h"
#include "ncurses.h"
#include "screen.h"
#include "time.h"

//
//
// C E L L
Cell::Cell(size_t _x = 0, size_t _y = 0): x(_x), y(_y), cellType(EMPTY) {}
void Cell::setCellType(CellType _cellType) {
    this->cellType = _cellType;
}
bool Cell::operator==(const Cell &cell) {
    if (this->x == cell.x && this->y == cell.y)
        return true;
    return false;
}
CellType Cell::getCellType() {
    return this->cellType;
}
size_t Cell::getX() {
    return this->x;
}
size_t Cell::getY() {
    return this->y;
}
void Cell::set(size_t x, size_t y) {
    this->x = x;
    this->y = y;
}

//
//
// B O A R D
Board::Board(size_t window_x, size_t window_y): x(window_x), y(window_y) {
    this->cells = new Cell*[window_x];

    for (size_t x = 0; x < window_x; ++x)
        this->cells[x] = new Cell[window_y]; // TODO may cause mem leak => check later

    for (size_t x = 0; x < window_x; ++x)
        for (size_t y = 0; y < window_y; ++y)
            this->cells[x][y].set(x,y);
}
Board::~Board() {
    for (size_t x= 0; x < this->x; ++x)
        delete this->cells[x];
    delete this->cells;
}
Cell ** Board::getCells() {
    return this->cells;
}
Cell & Board::getCell(size_t x, size_t y) {
    return this->cells[x][y];
}
void Board::generateFood() {
    // TODO generate food
}

//
//
// S N A K E
//class Snake {
//    vector<Cell> bodyParts;
//    Cell head;
//public:
//    Snake(Cell &initPos);
//    void grow();
//    void move(Cell& nextCell);
//    bool checkCrash(Cell& nextCell);
//    vector<Cell>& getBodyParts();
//};
Snake::Snake(Cell &initPos)
{
    head = initPos;
    snakeParts.push_back();
    head.setCellType(SNAKE_NODE);
}
vector<Cell> & Snake::getBodyParts() {
    return this->snakeParts;
}
void Snake::grow() {

}
void Snake::move(Cell &nextCell) {
    Cell tail = this->snakeParts.pop_back();
    tail.setCellType(EMPTY);

    head = nextCell;
    head.setCellType(SNAKE_NODE);
    this->snakeParts.insert(this->snakeParts.begin(), head);
}
bool Snake::checkCrash(Cell &nextCell) {

    for (auto &cell : this->snakeParts)
    {
        if ( cell == nextCell )
        {
            return true;
        }
    }

    return false;
}

CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c)
{

}
void CSnake::paint()
{
    CFramedWindow::paint();

}

bool CSnake::handleEvent(int key)
{

    switch (key)
    {
        case KEY_UP:
            return true;
        case KEY_DOWN:
            return true;
        case KEY_RIGHT:
            return true;
        case KEY_LEFT:
            return true;
    };
    return false;
}
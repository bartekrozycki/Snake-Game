#include "snake.h"
#include "ncurses.h"
#include "screen.h"

#define PRE_ALLOC_SNAKE_LENGTH 5
#define SECOND 1000/20

static size_t tick = 0;

Snake::Snake(CRect _border, CPoint spawnPoint, char char_head, char char_body)
{
    this->border = _border;
    this->c_body = char_body;
    this->c_head = char_head;
    this->spawnPoint = spawnPoint;

    this->init();
}
void Snake::init() {
    this->body.clear();
    this->body.insert(body.begin(), spawnPoint);
    this->body.resize(PRE_ALLOC_SNAKE_LENGTH);
    this->dir = Direction::RIGHT;
}
void Snake::changeDirection(Direction dir) {
    switch (this->last_dir)
    {
        case Direction::RIGHT:
            if (dir == Direction::LEFT) return;
            break;
        case Direction::LEFT:
            if (dir == Direction::RIGHT) return;
            break;
        case Direction::UP:
            if (dir == Direction::DOWN) return;
            break;
        case Direction::DOWN:
            if (dir == Direction::UP) return;
            break;
    }
    this->dir = dir;
}
void Snake::move() {
    CPoint head = body.front();
    size_t temp;
    switch (this->dir) {
        case Direction::RIGHT:
            temp = head.x % (this->border.size.x - 2) + 1;
            body.insert(body.begin(), CPoint(temp, head.y));
            break;
        case Direction::LEFT:
            temp = head.x - 1 == 0 ? this->border.size.x - 2 : head.x - 1;
            body.insert(body.begin(), CPoint(temp, head.y));
            break;
        case Direction::UP:
            temp = head.y - 1 == 0 ? this->border.size.y - 2 : head.y - 1;
            body.insert(body.begin(), CPoint(head.x, temp));
            break;
        case Direction::DOWN:
            temp = head.y % (this->border.size.y - 2) + 1;
            body.insert(body.begin(), CPoint(head.x, temp));
            break;
    }
    this->last_dir = this->dir;
    body.pop_back();
}
void Snake::paint() {
    auto i = body.cbegin();
    gotoyx(this->border.topleft.y + i -> y, this->border.topleft.x + i -> x);
    printc(c_head);

    for (++i; i != body.cend(); ++i) {
        gotoyx(this->border.topleft.y + i -> y, this->border.topleft.x + i -> x);
        printc(c_body);
    }
}

//////////        ###
////////         #
////////         #
//////////        ### SNAKE

CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c), player(r)
{

}
void CSnake::paint()
{
    CFramedWindow::paint();
    player.paint();
    if (gameMode == GameMode::HELP_MODE)
        this->paintHelp();

    gotoyx(this->geom.topleft.y + 0, this->geom.topleft.x + 2);
    printl("[ Score: %u ]---[ Level %u ]", player.score, player.level);
}
void CSnake::paintHelp() {
    gotoyx(this->geom.topleft.y + 5, this->geom.topleft.x + 10);
    printl("Press 'h' to toggle help information.");
    gotoyx(this->geom.topleft.y + 6, this->geom.topleft.x + 10);
    printl("Press 'p' to toggle pause/play mode.");
    gotoyx(this->geom.topleft.y + 7, this->geom.topleft.x + 10);
    printl("Press 'r' to restart game.");
    gotoyx(this->geom.topleft.y + 9, this->geom.topleft.x + 12);
    printl("Use arrows to move snake (in play mode) or");
    gotoyx(this->geom.topleft.y + 10, this->geom.topleft.x + 10);
    printl("move window (in pause mode)");
}
bool CSnake::handleEvent(int key)
{
    switch (key)
    {
        case 'p':
            gameMode = (gameMode == GameMode::PLAY_MODE) ? GameMode::PAUSE_MODE : GameMode::PLAY_MODE;
            return true;
        case 'h':
            gameMode = (gameMode == GameMode::PLAY_MODE) ? GameMode::HELP_MODE : GameMode::PLAY_MODE;
            return true;
        case 'r':
            player.init();
            return true;
    };

    if (gameMode != GameMode::PLAY_MODE)
    {
        paint();

        if (CFramedWindow::handleEvent(key))
            return true;

        return false;
    }

    ++tick;
    if (!(tick % (SECOND / 4)))
    {
        player.move();
        paint();
    }
    switch (key)
    {
        case KEY_UP:
            player.changeDirection(Snake::Direction::UP);
            return true;
        case KEY_DOWN:
            player.changeDirection(Snake::Direction::DOWN);
            return true;
        case KEY_RIGHT:
            player.changeDirection(Snake::Direction::RIGHT);
            return true;
        case KEY_LEFT:
            player.changeDirection(Snake::Direction::LEFT);
            return true;
    };
    return false;
}
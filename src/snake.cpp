#include "snake.h"
#include "ncurses.h"
#include "screen.h"
#include <time.h>
#include <cmath>
#include <algorithm>
#include <set>


#define PRE_ALLOC_SNAKE_LENGTH 1
#define SECOND 1000/20

static size_t tick = 0;

Snake::Snake(CRect _border, CPoint spawnPoint, char char_head, char char_body)
{
    this->border = _border;
    this->c_body = char_body;
    this->c_head = char_head;
    this->spawnPoint = spawnPoint;

    srand( (unsigned)time(NULL) );

    this->init();
}
void Snake::init() {
    this->body.clear();
    this->body.push_front(spawnPoint);
    this->body.resize(PRE_ALLOC_SNAKE_LENGTH);
    this->dir = Direction::RIGHT;
    this->food.clear();
    this->score = 0;
    this->spawnFood();
}
void Snake::spawnFood() {
    CPoint entity;
    bool found = 0;
    while (!found)
    {
        entity = CPoint(((rand()%(this->border.size.x - 2)) + 1), ((rand()%(this->border.size.y - 2)) + 1));

        for (auto& cell : this->body)
        {
            if (!(entity == cell))
            {
                found = 1;
                break;
            }
        }

    }
    this->food.push_back(entity);
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
void Snake::setBorder(CRect &rect) {
    this->border = rect;
}
CPoint Snake::nextCell(Direction d) {
    CPoint next;
    CPoint head = body.front();
    size_t temp;

    switch (d) {
        case Direction::RIGHT:
            temp = (head.x % (this->border.size.x - 2)) + 1;
            next = CPoint(temp, head.y);
            break;
        case Direction::LEFT:
            temp = head.x - 1 == 0 ? this->border.size.x - 2 : head.x - 1;
            next = CPoint(temp, head.y);
            break;
        case Direction::UP:
            temp = head.y - 1 == 0 ? this->border.size.y - 2 : head.y - 1;
            next = CPoint(head.x, temp);
            break;
        case Direction::DOWN:
            temp = (head.y % (this->border.size.y - 2)) + 1;
            next = CPoint(head.x, temp);
            break;
    }

    return next;
}
bool Snake::move() {
    CPoint head = nextCell(this->dir);
    body.push_front(head);

    bool eat = false;
    for (auto& cell : this->food)
    {
        if (head.x == cell.x && head.y == cell.y)
        {
            eat = true;
            score+=1;
            this->food.pop_back();
            this->spawnFood();
        }
    }
    this->last_dir = this->dir;
    if (!eat) body.pop_back();

    return this->checkCrash(head);
}
bool Snake::checkCrash(const CPoint &head) const{

    auto it = find_if(this->body.begin() + 1, this->body.end(), [head]
    (const CPoint &ref) -> bool {
        return ref.operator==(head);
    });

    return it != this->body.end();
}
void Snake::paint() {
    auto i = body.cbegin();
    gotoyx(this->border.topleft.y + i -> y, this->border.topleft.x + i -> x);
    printc(c_head);

    for (++i; i != body.cend(); ++i) {
        gotoyx(this->border.topleft.y + i -> y, this->border.topleft.x + i -> x);
        printc(c_body);
    }
    for (auto& cell : this->food)
    {
        gotoyx(this->border.topleft.y + cell.y, this->border.topleft.x + cell.x);
        printc('$');
    }

}

size_t Snake::getLevel() {
    return ( this->score / 5 );
}

void Snake::ai() {

    CPoint &food = this->food.front();
    CPoint &head = this->body.front();

    gotoyx(0,0);
    printl("FOOD x %d y %d", food.x, food.y);
    gotoyx(1,0);
    printl("HEAD x %d y %d", head.x, head.y);

    CPoint moveUp = nextCell(Direction::UP);
    CPoint moveRight = nextCell(Direction::RIGHT);
    CPoint moveDown = nextCell(Direction::DOWN);
    CPoint moveLeft = nextCell(Direction::LEFT);

    bool canUp = checkCrash(moveUp);
    bool canRight = checkCrash(moveRight);
    bool canDown = checkCrash(moveDown);
    bool canLeft = checkCrash(moveLeft);

    gotoyx(2,0);
    printl("[U R D L] => [%d %d %d %d]", canUp, canRight, canDown, canLeft);

    set<pair<int,CPoint>> possible_moves;
    possible_moves.insert(make_pair(this->food.begin()->dist(moveUp), moveUp));
    possible_moves.insert(make_pair(this->food.begin()->dist(moveRight), moveRight));
    possible_moves.insert(make_pair(this->food.begin()->dist(moveDown), moveDown));
    possible_moves.insert(make_pair(this->food.begin()->dist(moveLeft), moveLeft));


    for (auto &i : possible_moves)
    {
        if (!checkCrash(i.second))
        {
            if (i.second == moveUp)
                changeDirection(Direction::UP);
            else if (i.second == moveRight)
                changeDirection(Direction::RIGHT);
            else if (i.second == moveDown)
                changeDirection(Direction::DOWN);
            else if (i.second == moveLeft)
                changeDirection(Direction::LEFT);

            break;
        }
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


    gotoyx(this->geom.topleft.y + 0, this->geom.topleft.x + 2);
    printl("[ Score: %u ]---[ Level %u ]", player.score, player.getLevel());
    player.setBorder(this->geom);
    player.paint();
    if (gameMode == GameMode::HELP_MODE)
        this->paintHelp();
    if (gameMode == GameMode::GAME_OVER)
        this->paintGameOver();
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
void CSnake::paintGameOver() {
    gotoyx(this->geom.topleft.y + 5, this->geom.topleft.x + 15);
    printl("G A M E   O V E R");
    gotoyx(this->geom.topleft.y + 6, this->geom.topleft.x + 15);
    printl("Your score: %d", player.score);
    gotoyx(this->geom.topleft.y + 7, this->geom.topleft.x + 15);
    printl("Press 'r' to restart game.", player.score);
}
bool CSnake::handleEvent(int key)
{
    if (gameMode == GameMode::GAME_OVER)
    {
        if (key == 'r')
        {
            player.init();
            gameMode = GameMode::PLAY_MODE;
            return true;
        }
        return false;
    }

    if (gameMode == GameMode::AI_MODE)
    {
        if (key == 'a')
        {
            gameMode = GameMode::PLAY_MODE;
            return true;
        }
        this->player.ai();
        ++tick;
        if (!(tick % (1)))
        {
            if (player.move())
            {
                this->gameMode = GameMode::GAME_OVER;
            }
            paint();
        }

        return false;
    }

    switch (key)
    {
        case 'a':
            gameMode = GameMode::AI_MODE;
            return true;
        case 'p':
            gameMode = (gameMode == GameMode::PLAY_MODE) ? GameMode::PAUSE_MODE : GameMode::PLAY_MODE;
            return true;
        case 'h':
            gameMode = (gameMode == GameMode::PLAY_MODE) ? GameMode::HELP_MODE : GameMode::PLAY_MODE;
            return true;
        case 'r':
            player.init();
            return true;
//        case 'f':
//            player.spawnFood();
//            return true;
    };

    if (gameMode != GameMode::PLAY_MODE)
    {
        paint();

        if (CFramedWindow::handleEvent(key))
            return true;

        return false;
    }

    ++tick;
    if (!(tick % (SECOND / ((size_t) (4 + (player.getLevel()) ) ) + 1)))
    {
        if (player.move())
        {
            this->gameMode = GameMode::GAME_OVER;
        }
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
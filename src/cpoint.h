#ifndef __C_POINT_H__
#define __C_POINT_H__

#include <list>
#include <math.h>

using namespace std;

struct CPoint
{
    int x;
    int y;
    CPoint(int _x=0, int _y=0): x(_x), y(_y) {};
    CPoint& operator+=(const CPoint& delta)
    {
        x+=delta.x;
        y+=delta.y;
        return *this;
    }
    bool operator==(const CPoint& lp) const {
        if (this->x == lp.x && this->y == lp.y) return true;
        return false;
    }
    CPoint operator+(const CPoint& lp) const
    {
        return CPoint(x + lp.x, y + lp.y);
    }
    int dist(const CPoint &n) const
    {
        return (int)sqrt( pow((n.x - this->x),2) + pow(n.y - this->y,2) );
    }
    bool operator<(const CPoint &n) const {
        return (sqrt( pow((this->x),2) + pow(this->y,2)) < sqrt( pow((n.x),2) + pow(n.y,2)));
    }
};

struct CRect
{
    CPoint topleft;
    CPoint size;
    CRect(CPoint t1 = CPoint(), CPoint s=CPoint()) : topleft(t1), size(s) {};
};

#endif

#ifndef __C_POINT_H__
#define __C_POINT_H__

#include <list>

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
    CPoint operator+(const CPoint& lp) const
    {
        return CPoint(x + lp.x, y + lp.y);
    }
};

struct CRect
{
    CPoint topleft;
    CPoint size;
    CRect(CPoint t1=CPoint(), CPoint s=CPoint()):
            topleft(t1), size(s) {};
};

#endif

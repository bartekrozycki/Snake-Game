#include "winsys.h"
#include "snake.h"

int main ()
{
    CDesktop d;
    d.insert(new CInputLine (CRect (CPoint (5, 7), CPoint (15, 15))));
    d.insert(new CWindow (CRect (CPoint (2, 3), CPoint (20, 10)), '#'));
    d.insert(new CSnake(CRect (CPoint (25, 2), CPoint (60, 25))));
    d.run ();
    return 0;
}

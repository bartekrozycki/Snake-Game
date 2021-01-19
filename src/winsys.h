#ifndef __WINSYS_H__
#define __WINSYS_H__

#include "cpoint.h"
#include <list>
#include <string>
//#include <ctype.h>
//#include <stdarg.h>
using namespace std;

class CView
{
protected:
    CRect geom;

public:
    explicit CView(CRect g) : geom(g) {}
    virtual ~CView() = default;

    virtual void paint () = 0; // abstract
    virtual bool handleEvent (int key) = 0; // abstract
    virtual void move (const CPoint & delta);
};

class CWindow:public CView
{
protected:
    char c;
public:
    CWindow(CRect r, char _c = '*') : CView(r), c(_c) {}

    void paint() override; // override abstract
    bool handleEvent(int key) override; // override abstract
};

class CFramedWindow: public CWindow
{
public:
    explicit CFramedWindow(CRect r, char _c = '\'') : CWindow(r, _c) {}

    void paint() override;
};

class CInputLine:public CFramedWindow
{
    string text;
public:
    CInputLine(CRect r, char _c = ',') : CFramedWindow(r, _c) {}

    void paint() override;
    bool handleEvent(int c) override;
};

class CGroup:public CView
{
    list< CView * > children;
public:
    explicit CGroup(CRect g) : CView(g) {}
    ~CGroup() override;

    void paint() override;
    bool handleEvent(int key) override;
    void insert(CView * v);
};

class CDesktop:public CGroup
{
public:
    CDesktop();
    ~CDesktop() override;

    void paint() override;
    int getEvent();
    void run();
};

#endif

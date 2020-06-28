#include "fireworks.h"
const double DBLPI = 6.28318530717958;
const double PI = 3.14159265359;

fireworks * fireworks::mgr = nullptr;

fireworks::fireworks(int n)
{
    nChild = n;
    starList.empty();
    starList.reserve(900000);
}

void fireworks::setFireWorks(int x, int y, int lv, float spd, float dir, int delay)
{
    starList.push_back(stars(x,y,lv,spd,dir,delay));
}

void fireworks::draw(HWND hWnd,HDC hMemdc)
{
    RECT scrBx;
    GetClientRect(hWnd,&scrBx);

    HPEN noPen = (HPEN)GetStockObject(NULL_PEN);
    HPEN oldPen = (HPEN)SelectObject(hMemdc, noPen);
    //Ellipse(hMemdc, 300 - 20, 500  - 20, 300 +20, 500 + 20);
    HBRUSH noBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hMemdc, noBrush);
    const int nclrs = 3;
    HBRUSH brs[nclrs] = {
        CreateSolidBrush(RGB(0xFF,0x00,0x00)),
        CreateSolidBrush(RGB(0x00,0xFF,0x00)),
        CreateSolidBrush(RGB(0x00,0x00,0xFF))
    };
    clock_t now = clock();
    int cp = 0;
    int vlen = starList.size();
    auto iter = starList.begin();
    for (int j = 0; j < vlen; j++) {
        if (now > ((iter + j)->startT + (iter + j)->delayed)){
            if ((iter + j)->delayed > 0 && (iter+j)->delayed > 0) {
                (iter + j)->explode(starList, nChild);
                iter = starList.begin();
                (iter + j)->delayed = -1;
                (iter + j)->startT = -1;
                vlen = starList.size();
            }
        }
    }
    for (auto i = starList.begin(); i != starList.end(); i++) {
        if (i->delayed < 0 || i->startT < 0) continue;
        double s = (now - i->startT) * (i->speed);
        int x = i->pos.x + (s * cos(i->direction));
        int y = i->pos.y - (s * sin(i->direction));
        SelectObject(hMemdc, brs[cp % 3]);
        Ellipse(hMemdc, x - 2, y - 2, x + 2, y + 2);
        cp++;
    }

    if (cp == 0) starList.empty();

    SelectObject(hMemdc, oldPen);
    DeleteObject(noPen);
    SelectObject(hMemdc, oldBrush);
    for (int i = 0; i < nclrs; i++) {
        DeleteObject(brs[i]);
    }
}


fireworks* fireworks::getMgr(int n)
{
    if (fireworks::mgr == nullptr) {
        fireworks::mgr = new fireworks(n);
    }
    return fireworks::mgr;
}

void fireworks::freeMgr()
{
    if (mgr == nullptr) return;
    fireworks* tmp = mgr;
    mgr = nullptr;
    delete tmp;
}

stars::stars(int x, int y, int lv, float spd, float dir, int delay)
{
    pos.x = x;
    pos.y = y;
    level = lv;
    speed = spd;
    direction = dir;
    startT = clock();
    delayed = delay;
}

void stars::explode(vector<stars>& sl,int nchild)
{
    if (level == 0) return ;
    if (delayed < 0) return;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < nchild; j++) {
            float s = delayed * speed;
            for (float r = 0; r < DBLPI; r += DBLPI / nchild) {
                int x = pos.x + int(s * cos(direction));
                int y = pos.y - int(s * sin(direction));
                sl.push_back(stars(x, y, level - 1, speed-0.1*i*speed, r, delayed - 500));
            }
        }
    }
    return ;
}


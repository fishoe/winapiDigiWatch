#pragma once
#include <windows.h>
#include <time.h>
#include <vector>
#include <math.h>

using namespace std;

struct POS
{
	int x;
	int y;
};

class stars;

class fireworks
{
	vector<stars> starList;
	int nChild;
	static fireworks* mgr;
public:
	fireworks(int n);
	void setFireWorks(int x , int y, int lv, float spd, float dir, int delay);
	void draw(HWND hWnd, HDC hMemdc);
	static fireworks* getMgr(int n);
	static void freeMgr();
};

class stars
{
	POS pos; //start pos of star
	int level;
	float speed; //speed of this star ()
	float direction; // direction of this star (rad)
	clock_t startT; //
	int delayed; //delayed time to explode (ms)
public:
	friend fireworks;
	stars(int x, int y, int lv, float spd, float dir, int delay);
	void explode(vector<stars>& sl , int nchild);
};
#pragma once
#include "Sprite.h"
#include "Sprites.h"
using namespace std;


/*
	Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;


class Animation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;

public:

	Animation(int defaultTime=100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y, int alpha = 255);
	void RenderFrame(float x, float y, int frame, int alpha = 255);
	int GetcurrenFrame();
	void SetCurrenFrame(int currenframe);
	int size();
};

typedef Animation* LPANIMATION;
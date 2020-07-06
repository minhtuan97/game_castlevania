#pragma once
#include "GameObject.h"

class Brickmove:public GameObject
{
	#define speed	0.03f;
public:
	Brickmove();
	float x_min, x_max;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


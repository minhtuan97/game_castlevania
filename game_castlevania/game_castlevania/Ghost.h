#pragma once

#define GHOST_SPEED_X 0.05f
#define GHOST_GRAVITY 0.005f


#include "GameObject.h"
class Ghost : public GameObject
{
public:
	bool isHide = false;
	float x_left, x_right;
	Ghost();
	Ghost(float X, float Y, int Direction);
	virtual ~Ghost();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


};


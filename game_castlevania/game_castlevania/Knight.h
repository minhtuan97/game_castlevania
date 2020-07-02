#pragma once
#include "GameObject.h"

class Knight:public GameObject
{
	float start_x;
	float start_y;

public:
	//bool isIdle;
	float left_max, right_max;

	Knight(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


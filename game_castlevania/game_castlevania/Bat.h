#pragma once
#include "GameObject.h"

class Bat:public GameObject
{

	float start_x;
	float start_y;
	float y_de = NULL;
	
public:
	bool isIdle;
	float left_max, right_max, top_max, bot_max;
	Bat(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


#pragma once
#include "GameObject.h"
#define Zombie_GRAVITY			0.003f

class Zombie:public GameObject
{
	int nx_zombie = -1;
public:
	float x_de = NULL;
	float y_de = NULL;
	Zombie(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};


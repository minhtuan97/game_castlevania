#pragma once
#include "GameObject.h"
#include <ctime>

#define SKELETON_GRAVITY	0.002f
#define SKELETON_SPEED_Y	0.3f
#define SKELETON_SPEED_X	0.07f

class Skeleton:public GameObject
{
public:
	bool isattack;
	Skeleton(int n);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};


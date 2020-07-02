#pragma once
#include "GameObject.h"
class StairBot : public GameObject
{
public:
	int nx = 0;
	StairBot();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~StairBot();
};


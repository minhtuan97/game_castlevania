#pragma once
#include "GameObject.h"
class StairTop: public GameObject
{
public:	
	int nx = 0;
	StairTop();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~StairTop();
};


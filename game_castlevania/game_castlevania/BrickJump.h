#pragma once
#include "GameObject.h"

class BrickJump: public GameObject
{
	virtual void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};


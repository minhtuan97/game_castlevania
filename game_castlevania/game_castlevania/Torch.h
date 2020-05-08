#pragma once
#include "GameObject.h"

class Torch: public GameObject 
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


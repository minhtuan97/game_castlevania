#pragma once
#include "GameObject.h"
#include "Simon.h"
#include "CNumber.h"

class Board: public GameObject
{
	CNumber* heartCollect;

public:
	Board();
	void Render(Simon* simon, int state, int RemainingTime, GameObject* boss);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};



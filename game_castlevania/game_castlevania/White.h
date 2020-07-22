#pragma once
#include "GameObject.h"

class White:public GameObject
{
	bool isIdle = true;
	bool isWalke = false;
	bool isJump = false;

public:
	float xde, yde;
	White();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	~White();
};


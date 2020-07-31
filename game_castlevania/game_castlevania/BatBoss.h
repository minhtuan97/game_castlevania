#pragma once
#include "GameObject.h"
#include "Simon.h"
#include <cstdlib>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

class BatBoss:public GameObject
{
private:
	Simon* simon;
	Game* game;
	float yLine1, yLine2;
	float xLine1, xLine2;
	float xminLine2, xmaxLine2;

	int dyRepair = 0; 
	int dyRepairToAttack = 0; 

	bool isWaiting = true; 

	bool isAutoGo = false; 
	bool isGoUp = false; 
	bool isGoDown = false;
	bool isResetPositionParabol = false;
	float autoGoX_Distance = 0.0f, autoGoY_Distance = 0.0f;
	float a, b;
	DWORD timeBeginState = 0;
public:
	BatBoss();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int getAniId();
	float randomX();
	float float_randomX(float min, float max);
	void SetState(int state);
	void beAttack();
	~BatBoss();
};


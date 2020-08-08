#pragma once
#include "GameObject.h"
#include "Simon.h"
#include "Skeleton.h"

#define WHITE_WALKING_SPEED		0.08f 
#define WHITE_JUMP_SPEED_Y		0.5f
#define WHITE_JUMP_DEFLECT_SPEED 0.2f
#define WHITE_GRAVITY			0.003f
#define WHITE_DIE_DEFLECT_SPEED	 0.5f

#define  WHITE_STATE_IDLE_LEFT		0
#define  WHITE_STATE_IDLE_RIGHT		1
#define  WHITE_STATE_WALK_LEFT		2
#define  WHITE_STATE_WALK_RIGHT		3
#define  WHITE_STATE_JUMP_LEFT		4
#define  WHITE_STATE_JUMP_RIGHT		5

class White:public GameObject
{
	bool isIdle = true;
	bool isWalk = false;
	bool isJump = false;
	bool isGo = false;
	bool isBack = false;
	Simon* simon;
	Camera* cam;
	float denta;
	float x_backup;
	float y_backup;
	vector<Skeleton*> list_Skeleton;
	
public:
	float xde, yde;
	White();
	White(int nx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	int random(int min, int max);
	~White();
};


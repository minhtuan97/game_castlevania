#pragma once
#include "GameObject.h"

#define MONKEY_BBOX_WIDTH		16
#define MONKEY_BBOX_HEIGHT		16

#define	MONKEY_STATE_IDLE		0
#define	MONKEY_STATE_WALK		1
#define	MONKEY_STATE_JUMP		2

#define MONKEY_JUMP_SPEED_X		0.14f
#define MONKEY_JUMP_SPEED_Y		0.15f

#define MONKEY_WALK_SPEED_X		0.2f
#define MONKEY_WALK_SPEED_Y		0.12f

#define MONKEY_GRAVITY_SPEED	0.0012f

#define MONKEY_TIME_WAIT_ATTACK	380

class Monkey: public GameObject
{
	bool isIdle = true;
	bool isJump = false;
	bool isDisableJump = false;
	bool isHitted = false;
	bool isHide = false;

public:
	float xde, yde;
	Monkey();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	~Monkey();
};


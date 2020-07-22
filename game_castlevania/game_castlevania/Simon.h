#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Weapon.h"

#define SIMON_WALKING_SPEED		0.08f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.003f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE_LEFT		0
#define SIMON_STATE_IDLE_RIGHT		1
#define SIMON_STATE_WALK_LEFT		2
#define SIMON_STATE_WALK_RIGHT		3
#define SIMON_STATE_JUMP_LEFT		4
#define SIMON_STATE_JUMP_RIGHT		5
#define SIMON_STATE_ASCEND_LEFT		6
#define SIMON_STATE_ASCEND_RIGHT	7
#define SIMON_STATE_DESCEND_LEFT	8
#define SIMON_STATE_DESCEND_RIGHT	9
#define SIMON_STATE_HURT_LEFT		10
#define SIMON_STATE_HURT_RIGHT		11
#define SIMON_STATE_DEATH_LEFT		12
#define SIMON_STATE_DEATH_RIGHT		13
#define SIMON_STATE_INTRO			14
#define SIMON_STATE_COLORS_LEFT		15
#define SIMON_STATE_COLORS_RIGHT	16
#define SIMON_STATE_STANDING_LEFT	17
#define SIMON_STATE_STANDING_RIGHT	18

#define SIMON_ANI_IDLE_LEFT			0
#define SIMON_ANI_IDLE_RIGHT		9
#define SIMON_ANI_WALK_LEFT			1
#define SIMON_ANI_WALK_RIGHT		10
#define SIMON_ANI_JUMP_LEFT			2
#define SIMON_ANI_JUMP_RIGHT		11
#define SIMON_ANI_ASCEND_LEFT		3
#define SIMON_ANI_ASCEND_RIGHT		12
#define SIMON_ANI_ASCEND_IDLE_RIGHT	25
#define SIMON_ANI_ASCEND_IDLE_LEFT	28
#define SIMON_ANI_DESCEND_IDLE_LEFT	27
#define SIMON_ANI_DESCEND_IDLE_RIGHT 26
#define SIMON_ANI_DESCEND_LEFT		4
#define SIMON_ANI_DESCEND_RIGHT		13
#define SIMON_ANI_HURT_LEFT			5
#define SIMON_ANI_HURT_RIGHT		14
#define SIMON_ANI_DEATH_LEFT		6
#define SIMON_ANI_DEATH_RIGHT		15
#define SIMON_ANI_INTRO				7
#define SIMON_ANI_COLORS_LEFT		8
#define SIMON_ANI_COLORS_RIGHT		16
#define	SIMON_ANI_STANDING_LEFT		17
#define	SIMON_ANI_STANDING_RIGHT	21
#define SIMON_ANI_ASCEND_WHIP_RIGHT	23
#define SIMON_ANI_ASCEND_WHIP_LEFT	19
#define SIMON_ANI_DESCEND_WHIP_LEFT	20
#define SIMON_ANI_DESCEND_WHIP_RIGHT	24


#define SIMON_BIG_BBOX_WIDTH  15
#define SIMON_BIG_BBOX_HEIGHT 27

#define SIMON_SMALL_BBOX_WIDTH  13
#define SIMON_SMALL_BBOX_HEIGHT 15

#define SIMON_UNTOUCHABLE_TIME 5000

class Simon: public GameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	float start_x;			// initial position of SIMON at scene
	float start_y;
	bool isJump = false;
	bool isStanding = false;
	bool isColor = false;
	bool isWhip = false;
	int preframe = 0;
	int loopani=0;
	Whip* whip;
	vector<Weapon*> listWeapon;
	int nx_stair;
	static Simon* __instance;
	

public:
	bool isIdleOnStair;
	bool isOnStair;
	bool isUpStair;
	bool isDownStair;
	bool canClimbUpStair;
	bool canClimbDownStair;
	int direcStair;
	float xStair, yStair;
	float distanceX, distanceY;
	bool isAttact = false;

	Simon(float x = 0.0f, float y = 0.0f);
	static Simon* GetInstance();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Idle();
	void Jump();
	void Standing();
	void Color();
	void Reset();
	void StairDown();
	void StairUp();
	void IdleOnStair();
	void attackWeapon();

	void SetWhip(Whip* whiptemp);
	//void Set
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Unload();
};


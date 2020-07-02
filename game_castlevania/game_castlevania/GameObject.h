#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <unordered_map>
#include "Animation.h"
#include "Animations.h"
#include "Textures.h"
#include <algorithm>

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class GameObject;
typedef GameObject* LPGAMEOBJECT;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;
struct CollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	float dx, dy;		// *RELATIVE* movement distance between this object and obj
	//CollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }
	CollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}
	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};



class GameObject
{
public:
	int ID;
	float x;
	float y;

	float dx, dy;

	float vx=0;
	float vy=0;

	int nx;
	int state;

	DWORD dt;

	LPANIMATION_SET animation_set;


public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }

	int GetState() { return this->state; }

	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }


	void RenderBoundingBox();
	
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny);


	GameObject();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	~GameObject();
};


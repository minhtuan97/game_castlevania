#include "Whip.h"


void Whip::Render()
{
	animation_set->at(1)->Render(x, y);
	//RenderBoundingBox();
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{

	GameObject::Update(dt);

	// Simple fall down
	vy += 0.0002f * dt;
	vx += 0.0002f * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);
	DebugOut(L"coevents.size=: %d\n", coEvents.size());
	//DebugOut(L"x:%f y:%f\n", x,y);
	if (coEvents.size() == 0)
	{
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		int sumtorch = -1;
		 //Collision logic with brick
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Torch*>(e->obj)) // if e->obj is Goomba 
				//DebugOut(L"Co va cham Torch, coevents.size=\n");
				sumtorch++;
				//delete e->obj;

		}
		DebugOut(L"So luong Torch va cham: %d\n",sumtorch);

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Whip::Render(int level,int nx, int frame, int alpha)
{
	switch (level)
	{
	case 1:
		if (nx > 0)
			animation_set->at(1)->RenderFrame(x, y, frame, alpha);
		else
			animation_set->at(0)->RenderFrame(x, y, frame, alpha);
		break;
	default:
		break;
	}
	//RenderBoundingBox();
}

void Whip::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 24;
	b = y + 16;
}
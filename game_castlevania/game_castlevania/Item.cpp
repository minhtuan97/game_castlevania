#include "Item.h"
#include"define.h"
#include "Brick.h"

Item::Item()
{
	isEnable = false;
	isfinish = false;
	type = 1;

}

Item::~Item()
{
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = x + 16;
	top = y;
	bottom = top + 16;
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);
	vy += 0.001f;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0)
		{
			vy = 0;
		}
		 //Collision logic with brick
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj))
				vy = 0;
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	/*if(type==4)
	{
		vx = vy = 0;
	}*/
}

void Item::Render()
{
		animation_set->at(type)->Render(x, y);
		RenderBoundingBox();
}

#include "Skeleton.h"
#include "Brick.h"
#include "Grid.h"
#include "Whip.h"
#include "Simon.h"
#include "StairTop.h"
#include "StairBot.h"
#include "BrickJump.h"
#include "Map.h"

Skeleton::Skeleton(int n)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATIONSET_SKELETON);
	SetAnimationSet(ani_set);
	nx = n;
	vx = SKELETON_SPEED_X*nx;
	vy = -SKELETON_SPEED_Y;
	isattack = true;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	Simon* simon = Simon::GetInstance();
	float xsimon, ysimon;
	simon->GetPosition(xsimon, ysimon);

	Grid* grid = Grid::GetInstance();
	// Calculate dx, dy 
	GameObject::Update(dt);

	// Simple fall down
	vy += SKELETON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vector<LPGAMEOBJECT> list;
	for (int i = 0; i < colliable_objects->size(); i++)
	{
		if (!(dynamic_cast<Skeleton*>(colliable_objects->at(i)))
			&& !(dynamic_cast<Whip*>(colliable_objects->at(i))) 
			&& !(dynamic_cast<StairTop*>(colliable_objects->at(i)))
			&& !(dynamic_cast<StairBot*>(colliable_objects->at(i)))
			&& !(dynamic_cast<BrickJump*>(colliable_objects->at(i))))
		{
			list.push_back(colliable_objects->at(i));
		}
	}

	coEvents.clear();

	CalcPotentialCollisions(&list, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj))
			{
				if (e->obj->y > ysimon)
					isattack = false;
				else
				{
					x += dx;
					y += dy;
				}
			} 
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	Map* map = Map::GetInstance();
	if (!isattack||y> y > map->GetHeight())
		grid->deleteObject(this);
	grid->Update(this);

}

void Skeleton::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y + BOARD_HEIGHT);
	//RenderBoundingBox();
}

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
	
}



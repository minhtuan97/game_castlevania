#include "Ghost.h"
#include "Brick.h"
#include "Map.h"
#include "Grid.h"
#include "Simon.h"

Ghost::Ghost()
{
}

Ghost::Ghost(float X, float Y, int Direction)
{
	x = X;
	y = Y;
	nx = Direction;

	//Health = 1; // sét máu

}

Ghost::~Ghost()
{
}

void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float xsimon, ysimon;
	Simon* simon = Simon::GetInstance();
	simon->GetPosition(xsimon, ysimon);
	if (xsimon > x_right || xsimon < x_left)
	{
		isHide = false;
		return;
	}
	else
	{
		isHide = true;
	}
	vx = GHOST_SPEED_X * nx;

	GameObject::Update(dt);
	
	
	if (y > ysimon - 6)
		vy = 0;
	else
		vy += GHOST_GRAVITY * dt;

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<Brick*>(coObjects->at(i)))
			listObject_Brick.push_back(coObjects->at(i));

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm 
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0)
		{
			//vx *= -1;
			nx *= -1;
		}

		if (ny != 0)
		{
			vy = 0;
			//vx = 0;
		}
	}
	if ((x < x_left && vx < 0) || (x > x_right && vx > 0))
		nx *= -1;

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	Map* map = Map::GetInstance();
	Grid* grid = Grid::GetInstance();
	if (x > map->GetWidth() - 10)
		grid->deleteObject(this);
	else
		grid->Update(this);
}

void Ghost::Render()
{
	if (!isHide) return;
	if (nx > 0)
		animation_set->at(1)->Render(x, y);
	else
		animation_set->at(0)->Render(x, y);
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
}

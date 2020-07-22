#include "Zombie.h"
#include "Brick.h"
#include "Map.h"
#include "Grid.h"
#include "Utils.h"

Zombie::Zombie(float x, float y)
{
	this->x = x;
	this->y = y;
	x_de = x;
	y_de = y;
	nx_zombie = -1;
	vx = 0.03f;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	float camX, camY;
	Camera* cam = Camera::GetInstance();
	camX = cam->GetCameraPosition().x;
	camY = cam->GetCameraPosition().y;

	//if (x < camX || x + MONKEY_BBOX_WIDTH > camX + SCREEN_WIDTH)
	//DebugOut(L"camX:%f, x:%f\n", camX, x);
	/*if (camX != 0 && camX<x - 50 && camX + SCREEN_WIDTH> x + MONKEY_BBOX_WIDTH)
	{
		DebugOut(L"vao setstate\n");

		if (!isJump)
			SetState(MONKEY_STATE_JUMP);
	}
	else
	{
		isHide = true;
	}
	if (y < yde - 50 && isJump)
		vy = MONKEY_JUMP_SPEED_Y;*/
	vx = 0.03f;
	vy = 0.06f;

	GameObject::Update(dt);

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < colliable_objects->size(); i++)
		if (dynamic_cast<Brick*>(colliable_objects->at(i)))
			listObject_Brick.push_back(colliable_objects->at(i));

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	//DebugOut(L"truoc luc va cham vy=%f, dy:%f\n", vy, dy);

	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm 
	if (coEvents.size() == 0)
	{
		y += dy;
		x -= dx;
		//DebugOut(L"sau khi va cham vy=%f, dy:%f\n",vy, dy);

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += nx_zombie * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0)
		{
			vx *= -1;
			nx *= -1;
		}

		if (ny != 0)
		{
			vy = 0;
			vx = 0;
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj))
			{
				//x -= dx;
				//DebugOut(L"dx=%f, nx=%d\n",dx, nx_zombie);
			}
		}
	}
	//if ((x < x_left && vx < 0) || (x > x_right && vx > 0))
	//	nx *= -1;

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	Map* map = Map::GetInstance();
	Grid* grid = Grid::GetInstance();
	if (x <10)
		grid->deleteObject(this);
	else
		grid->Update(this);
}

void Zombie::Render()
{
	int ani = 0;
	if (nx_zombie > 0)
		ani = 1;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void Zombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 32;
}

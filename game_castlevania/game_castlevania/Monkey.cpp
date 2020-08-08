#include "Monkey.h"
#include "define.h"
#include "Brick.h"
#include "Map.h"
#include "Grid.h"
#include "Utils.h"
#include "Simon.h"

Monkey::Monkey()
{
	nx = 1;
	SetState(MONKEY_STATE_IDLE);
	denta = 100;
}

void Monkey::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float camX, camY;
	Simon* simon = Simon::GetInstance();
	float xsimon, ysimon;
	simon->GetPosition(xsimon, ysimon);

	Camera* cam = Camera::GetInstance();
	camX=cam->GetCameraPosition().x;
	camY= cam->GetCameraPosition().y;
	
	//if (abs(x - xsimon) > denta)
	//{
	//	vx = -vx;

	//}
	if (x - xsimon > denta)
	{
		nx = -1;

	}
	if (x - xsimon > -denta)
	{
		nx = 1;

	}

	//if (x < camX || x + MONKEY_BBOX_WIDTH > camX + SCREEN_WIDTH)
	//DebugOut(L"camX:%f, x:%f\n", camX, x);
	if (camX!=0&&camX<x-50 && camX + SCREEN_WIDTH> x + MONKEY_BBOX_WIDTH)
	{
		//DebugOut(L"vao setstate\n");

		if(!isJump)
			SetState(MONKEY_STATE_JUMP);
	}
	else
	{
		isHide = true;
	}
	if (y < yde - 50 && isJump)
		vy = MONKEY_JUMP_SPEED_Y;


	GameObject::Update(dt);

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<Brick*>(coObjects->at(i))&& (&coObjects->at(i)!=NULL))
			listObject_Brick.push_back(coObjects->at(i));

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	//DebugOut(L"truoc luc va cham vy=%f, dy:%f\n", vy, dy);

	if (vy>0)
	{
		CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm 
	}
		if (coEvents.size() == 0)
		{
			y += dy;
			x += dx*nx;
			//DebugOut(L"sau khi va cham vy=%f, dy:%f\n",vy, dy);

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
				vx = 0;
			}
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Brick*>(e->obj))
				{

					isIdle = true;
					isJump = false;
					//this->nx = -this->nx;
					//vy = 0.05f;
				}
			}
		}
		//if ((x < x_left && vx < 0) || (x > x_right && vx > 0))
		//	nx *= -1;

		for (UINT i = 0; i < coEvents.size(); i++)
			delete coEvents[i];
	Map* map = Map::GetInstance();
	Grid* grid = Grid::GetInstance();
	if (x > map->GetWidth() - 10||y<1||x<1||y>map->GetHeight())
		grid->deleteObject(this);
	else
		grid->Update(this);



}

void Monkey::Render()
{
	int ani = 1;
	if (state == MONKEY_STATE_IDLE)
		ani = 1;
	if (state == MONKEY_STATE_JUMP)
		ani = 3;
	animation_set->at(ani)->Render(x, y + BOARD_HEIGHT);
	//RenderBoundingBox();
}

void Monkey::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
}

void Monkey::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MONKEY_STATE_IDLE:
		vx = 0;
		vy = 0;
		isJump = false;
		break;
	case MONKEY_STATE_WALK:
		vx = MONKEY_WALK_SPEED_X*nx;
		vy = -MONKEY_WALK_SPEED_Y;
		isJump = true;
		break;
	case MONKEY_STATE_JUMP:
		vx = MONKEY_JUMP_SPEED_X*nx;
		vy = -MONKEY_JUMP_SPEED_Y;
		isJump = true;
		break;
	}
}

Monkey::~Monkey()
{
}

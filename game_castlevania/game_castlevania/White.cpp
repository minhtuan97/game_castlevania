#include "White.h"
#include "Utils.h"
#include "Item.h"
#include "Candle.h"
#include "BrickJump.h"
#include "StairTop.h"
#include "StairBot.h"
#include "Map.h"
#include "Grid.h"

White::White()
{
	if (nx > 0)
		SetState(WHITE_STATE_WALK_RIGHT);
	else
		SetState(WHITE_STATE_WALK_LEFT);
	denta = 32;

}

void White::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


	simon = Simon::GetInstance();
	cam = Camera::GetInstance();
	float xsimon, ysimon;
	simon->GetPosition(xsimon, ysimon);
	if (isJump)
	{

	}
	else
	{
		if (isGo) vx = WHITE_WALKING_SPEED;
		if (isBack) vx = -WHITE_WALKING_SPEED;
	}

	// Calculate dx, dy
		GameObject::Update(dt);

	// Simple fall down
	vy += WHITE_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> list;


	for (int i = 0; i < coObjects->size(); i++)
	{
		if (!(dynamic_cast<Item*>(coObjects->at(i)))
			|| !(dynamic_cast<Candle*>(coObjects->at(i)))
			|| !(dynamic_cast<StairTop*>(coObjects->at(i)))
			|| !(dynamic_cast<StairBot*>(coObjects->at(i))))
		{
			list.push_back(coObjects->at(i));
		}
	}

	if (abs(x - xsimon) > denta)
	{
		isGo = true;
		isBack = false;
		//SetState(WHITE_STATE_WALK_RIGHT);
	}
	else
	{
		isGo = false;
		isBack = true;
		isJump = false;
		//SetState(WHITE_STATE_WALK_LEFT);

	}
	if (isBack && isJump)
	{
		x = x - denta;
		isJump = false;
	}
	// turn off collision when die 
	//if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(&list, coEvents);

	// reset untouchable timer if untouchable time has passed
	/*if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}*/


	// No collision occured, proceed normally
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

			if (dynamic_cast<BrickJump*>(e->obj))
			{
				BrickJump* b = dynamic_cast<BrickJump*>(e->obj);

				//// jump on top >> kill Goomba and deflect a bit 
				//if (e->ny < 0)
				//{
				//	if (goomba->GetState() != GOOMBA_STATE_DIE)
				//	{
				//		goomba->SetState(GOOMBA_STATE_DIE);
				//		vy = -MARIO_JUMP_DEFLECT_SPEED;
				//	}
				//}
				this->nx = b->nx;
				DebugOut(L"b->nx=%d, vx%f\n", b->nx, this->vx);

				if (b->nx * this->vx >= 0)
				{
					if (b->nx > 0)
					{
						vy = -WHITE_JUMP_SPEED_Y;
						vx = WHITE_WALKING_SPEED;
						isJump = true;
					}
					else
					{
						vy = -WHITE_JUMP_SPEED_Y;
						vx = -WHITE_WALKING_SPEED;
						isJump = true;
					}
				}
				if (abs(x - xsimon) < denta)
					isJump = false;
			}
			/*if (dynamic_cast<Brick*>(e->obj))
			{
				if (state == WHITE_STATE_JUMP_RIGHT)
					SetState(WHITE_STATE_WALK_RIGHT);
			}*/
			//	else if (e->nx != 0)
			//	{
			//		if (untouchable == 0)
			//		{
			//			if (goomba->GetState() != GOOMBA_STATE_DIE)
			//			{
			//				if (level > MARIO_LEVEL_SMALL)
			//				{
			//					level = MARIO_LEVEL_SMALL;
			//					StartUntouchable();
			//				}
			//				else
			//					SetState(MARIO_STATE_DIE);
			//			}
			//		}
			//	}
			//}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"vx=%f, nx%f", vx, nx);
	Map* map = Map::GetInstance();
	Grid* grid = Grid::GetInstance();
	if (x > map->GetWidth() - 10 || y < 1 || x<1 || y>map->GetHeight())
		grid->deleteObject(this);
	else
		grid->Update(this);
}

void White::Render()
{
	int ani = 0;
	switch (state)
	{
	case WHITE_STATE_IDLE_LEFT:
		ani = 0;
	case WHITE_STATE_IDLE_RIGHT:
		ani = 1;
	case WHITE_STATE_JUMP_LEFT:
		ani = 0;
		break;
	case WHITE_STATE_JUMP_RIGHT:
		ani = 1;
		break;
	case WHITE_STATE_WALK_LEFT:
		ani = 2;
		break;
	case WHITE_STATE_WALK_RIGHT:
		ani = 3;
		break;
	default:
		break;
	}
	
	//DebugOut(L"animationset=%d", ani);

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void White::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 32;
}

void White::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case WHITE_STATE_IDLE_LEFT:
		vx = vy = 0;
		isJump = false;
		break;
	case WHITE_STATE_IDLE_RIGHT:
		vx = vy = 0;
		isJump = false;
		break;
	case WHITE_STATE_JUMP_LEFT:
		vy = -WHITE_JUMP_SPEED_Y;
		vx = -WHITE_WALKING_SPEED;
		isJump = true;
		break;
	case WHITE_STATE_JUMP_RIGHT:
		vy = -WHITE_JUMP_SPEED_Y;
		vx = WHITE_WALKING_SPEED;
		isJump = true;
		break;
	case WHITE_STATE_WALK_LEFT:
		vx = -WHITE_WALKING_SPEED;
		isWalk = true;
		break;
	case WHITE_STATE_WALK_RIGHT:
		vx = WHITE_WALKING_SPEED;
		isWalk = true;
		break;

	}
}

White::~White()
{
}

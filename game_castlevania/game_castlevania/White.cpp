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
	

}

White::White(int n)
{
	this->nx = n;
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
	Map* map = Map::GetInstance();
	Grid* grid = Grid::GetInstance();
	float xsimon, ysimon;
	simon->GetPosition(xsimon, ysimon);
	if (isJump)
	{
		//isGo = false;
		//isBack = false;
		//vx = WHITE_WALKING_SPEED;
	}
	else
	{
		if (isGo) vx = WHITE_WALKING_SPEED*nx;
		if (isBack) vx = -WHITE_WALKING_SPEED*nx;
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
			&& !(dynamic_cast<Candle*>(coObjects->at(i)))
			&& !(dynamic_cast<StairTop*>(coObjects->at(i)))
			&& !(dynamic_cast<StairBot*>(coObjects->at(i))))
		{
			list.push_back(coObjects->at(i));
		}
	}

	if (!isJump)
	{
		if (abs(x - xsimon) > denta)
		{
			isGo = true;
			isBack = false;
		}
		else
		{
			isGo = false;
			isBack = true;
			isJump = false;

		}
	}

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
			if (dynamic_cast<Brick*>(e->obj))
			{
				isJump = false;
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"vx=%f, nx%f", vx, nx);
	
	//khoi tao Skeleton

	if (list_Skeleton.empty())
	{
		
		int n = random(1,2);
		for (int i = 0; i < n; i++)
		{
			Skeleton* s = new Skeleton(this->nx);
			s->SetPosition(x+ WHITE_BBOX_WIDTH, y);
			if(i==1)
				s->SetPosition(x + WHITE_BBOX_WIDTH, y-WHITE_BBOX_HEIGHT/2);

			list_Skeleton.push_back(s);
			grid->addObject(s);
		}
		DebugOut(L"n=%d\n",n);
	}
	else
	{
		for (int i = 0; i < list_Skeleton.size(); i++)
		{
			
			if (!list_Skeleton.at(i)->isattack)
			{
				Skeleton* s = list_Skeleton.at(i);
				//grid->deleteObject(list_Skeleton.at(i));
				list_Skeleton.erase(std::remove(list_Skeleton.begin(), list_Skeleton.end(), s), list_Skeleton.end());
				//list_Skeleton.erase(list_Skeleton.begin() + i);
				i--;
				//delete s;
				//s = NULL;
			}

		}

	}

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

	animation_set->at(ani)->Render(x, y + BOARD_HEIGHT);

	//RenderBoundingBox();
	//for (int i = 0; i < list_Skeleton.size(); i++)
		//list_Skeleton.at(i)->Render();
}

void White::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WHITE_BBOX_WIDTH;
	bottom = y + WHITE_BBOX_HEIGHT;
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

int White::random(int min, int max)
{
	srand(time(0));
	int s = (int)(max - min + 1);
	int res = rand() % s + min;
	return res;
}

White::~White()
{
}

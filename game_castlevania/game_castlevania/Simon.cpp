#include "Simon.h"
#include "Utils.h"
#include "Brick.h"
#include "Torch.h"
#include "Item.h"
#include "PlayScene.h"
#include "define.h"
#include "Portal.h"
#include "StairBot.h"
#include "StairTop.h"
#include <cmath>
#include "Bat.h"
#include "Knight.h"
#include "Brickmove.h"

Simon* Simon::__instance = NULL;

Simon::Simon(float x, float y)
{
	level = 1;
	untouchable = 0;
	SetState(SIMON_STATE_IDLE_RIGHT);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

}

Simon* Simon::GetInstance()
{
	if (__instance == NULL) __instance = new Simon();
	return __instance;
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	//if (y < 1)y = 1;
	if (isStanding)
		vx = 0;
	if (isColor == true)
		vx = 0;
	// Calculate dx, dy 
	GameObject::Update(dt);
	if (isStanding||isWhip) dx =  0;
	if (x < 0)	x = 0;
	Map* map = Map::GetInstance();
	if (x > map->GetWidth()-44) 
		x = map->GetWidth() - 44;
	
	if (isOnStair)
	{
		if (!isIdleOnStair)
		{
			if (isUpStair)
			{
				vx = 0.01f;
				vy = -0.01f;
			}
			else if(isDownStair)
			{
				vx = -0.01f;
				vy = 0.01f;
			}
		}
		else
		{
			vx = vy = 0;
		}
		if (canClimbUpStair)
		{
			x = xStair;
			canClimbUpStair = false;
		}
		
	}
	else
	{
		vy += SIMON_GRAVITY * dt;
	}

	if (vy > 1)
		vy = 1;

	canClimbUpStair = false;
	for (int i = 0; i < colliable_objects->size(); i++)
	{
		if (dynamic_cast<StairTop*>(colliable_objects->at(i)))
		{
			StairTop* st = dynamic_cast<StairTop*>(colliable_objects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			st->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				if (!isOnStair)
				{
					canClimbDownStair = true;
					xStair = st->x + 16;
					yStair = st->y-8;
					direcStair = st->nx;
					//DebugOut(L"vao ham va cham voi top isOnStair=%d, isDownStair=%d, canClimbDownStair=%d\n", isOnStair, isDownStair, canClimbDownStair);
				}
				else
					if (isUpStair) {
						isOnStair = false;
						nx = nx_stair;
						canClimbDownStair = false;
						isDownStair = false;
						isUpStair = false;
						x = st->x;
						y= st->y; 
					}

			}
		}

		else if (dynamic_cast<StairBot*>(colliable_objects->at(i)))
		{
			StairBot* sb = dynamic_cast<StairBot*>(colliable_objects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			sb->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				if (!isOnStair)
				{
					canClimbUpStair = true;
					xStair = sb->x + 3;
					yStair = sb->y;
					direcStair = sb->nx;
				}
				else
					if (isDownStair) {
						isOnStair = false;
						nx = nx_stair;
						canClimbDownStair = false;
					}
				
			}
		}
		if (dynamic_cast<Knight*>(colliable_objects->at(i)))
		{
			Knight* knight = dynamic_cast<Knight*>(colliable_objects->at(i));
			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			knight->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				Grid* grid = Grid::GetInstance();
				grid->deleteObject(knight);
			}
		}
		if (dynamic_cast<Bat*>(colliable_objects->at(i)))
		{
			Bat* bat = dynamic_cast<Bat*>(colliable_objects->at(i));
			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			bat->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				Grid* grid = Grid::GetInstance();
				grid->deleteObject(bat);
			}
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DEATH_LEFT||state!= SIMON_STATE_DEATH_RIGHT)
		CalcPotentialCollisions(colliable_objects, coEvents);

	// reset untouchable timer if untouchable time has passed
	//if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}

	// No collision occured, proceed normally
	
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (dynamic_cast<StairTop*>(e->obj) || dynamic_cast<StairBot*>(e->obj))
		{
			coEvents.erase(coEvents.begin() + i);
		}
	}

	if (coEvents.size() == 0)
	{
		if (isOnStair)
		{
			if (!isIdleOnStair) {
				bool isMovingX = false, isMovingY = false;
				//DebugOut(L"vao1 dx=%f dy=%f distanceX=%f distanceY=%f vx=%f vy=%f\n", dx, dy, distanceX, distanceY, vx, vy);

				if (std::abs(dx) < distanceX && distanceX != 0)
				{
					x = x + dx;
					distanceX = distanceX- std::abs(dx);
				}
				else
				{
					if (dx > 0)
						x = x + distanceX;
					else
						x = x - distanceX;
					distanceX = 0;
					isMovingX = true;
				}
				if (std::abs(dy) < distanceY && distanceY != 0)
				{
					y = y + dy;
					distanceY = distanceY- std::abs(dy);
				}
				else
				{
					if (dy > 0)
						y = y + distanceY;
					else
						y = y - distanceY;

					distanceY = 0;
					isMovingY = true;
				}
				if (isMovingX && isMovingY)
					IdleOnStair();
			}
		}
		else
		{
			x += dx;
			y += dy;
		}
		
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
			isJump = false;
		}
		/*else
			isJump = true;*/

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj))
			{

				isJump = false;
				if (isDownStair)
				{
					//x -= 10;
					y += 8;
				}
			}
			if (dynamic_cast<Brickmove*>(e->obj))
			{
				Brickmove* brick = dynamic_cast<Brickmove*>(e->obj);
				isJump = false;
				//DebugOut(L"vx=%f, dx=%f\n", vx, dx);
				//x += dx;
				vx = brick->vx;
				dx = vx * (2*dt);
				x += dx;
				DebugOut(L"vx=%f, dx=%f\n", vx, dx);

			}
			
			
		}
		//Collision logic with item
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Item*>(e->obj))
			{
				Item* item = dynamic_cast<Item*>(e->obj);
				item->isfinish = true;
				if (item->GetTypeItem() == WHIP)
				{
					preframe = 0;
					loopani = 0;
					Color();
				}
				Grid* grid = Grid::GetInstance();
				grid->deleteObject(e->obj);
				
			}
			if (dynamic_cast<Portal*>(e->obj))
			{
				Portal* p = dynamic_cast<Portal*>(e->obj);
				Game::GetInstance()->SwitchScene(p->GetSceneId());
			}
					
		}
		
		//	// if Portal 
		//	else if (dynamic_cast<Portal*>(e->obj))
		//	{
		//		Portal* p = dynamic_cast<Portal*>(e->obj);
		//		Game::GetInstance()->SwitchScene(p->GetSceneId());
		//	}
		//}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//DebugOut(L"ket thuc ham update isOnStair=%d, isDownStair=%d, canClimbDownStair=%d\n", isOnStair, isDownStair, canClimbDownStair);


}

void Simon::Render()
{
	int alpha = 255;
	if (untouchable) alpha = 128;
	int ani = 0;
		switch (state)
		{
		case SIMON_STATE_IDLE_RIGHT:
			ani = SIMON_ANI_IDLE_RIGHT;
			break;
		case SIMON_STATE_IDLE_LEFT:
			ani = SIMON_ANI_IDLE_LEFT;
			break;
		case SIMON_STATE_WALK_RIGHT:
			ani = SIMON_ANI_WALK_RIGHT;
			break;
		case SIMON_STATE_WALK_LEFT:
			ani = SIMON_ANI_WALK_LEFT;
			break;
		default:
			break;
		}

		if (isOnStair)
		{
			int currentframe = -1;
			if (isIdleOnStair)
			{
				if (isUpStair)
					ani = SIMON_ANI_ASCEND_IDLE_RIGHT;
				else if (isDownStair)
					ani = SIMON_ANI_DESCEND_IDLE_LEFT;

			}
			else
			{
				if (isUpStair)
				{
					if (nx_stair > 0)
						ani = SIMON_ANI_ASCEND_RIGHT;
					else
						ani = SIMON_ANI_ASCEND_LEFT;
				}
				else if (isDownStair)
				{
					if (nx_stair > 0)
						ani = SIMON_ANI_DESCEND_RIGHT;
					else
						ani = SIMON_ANI_DESCEND_LEFT;
				}

			}

			if (isWhip == true)
			{
				if (isUpStair)
				{
					if (nx_stair > 0)
						ani = SIMON_ANI_ASCEND_WHIP_RIGHT;
					else
						ani = SIMON_ANI_ASCEND_WHIP_LEFT;
				}
				else if (isDownStair)
				{
					if (nx_stair > 0)
						ani = SIMON_ANI_DESCEND_WHIP_RIGHT;
					else
						ani = SIMON_ANI_DESCEND_WHIP_LEFT;
				}

				int currentframe = animation_set->at(ani)->GetcurrenFrame();
				if (currentframe > 2)
				{
					isWhip = false;
					animation_set->at(ani)->SetCurrenFrame(-1);
					if (isUpStair)
					{
						if (nx_stair > 0)
							ani = SIMON_ANI_ASCEND_RIGHT;
						else
							ani = SIMON_ANI_ASCEND_LEFT;
					}
					else if (isDownStair)
					{
						if (nx_stair > 0)
							ani = SIMON_ANI_DESCEND_RIGHT;
						else
							ani = SIMON_ANI_DESCEND_LEFT;
					}
				}
				else
				{
					float l, t, r, b;
					GetBoundingBox(l, t, r, b);
					if (nx_stair > 0)
						whip->SetPosition(l - 10, t);
					else
						whip->SetPosition(r, t);
					if (currentframe == 2)
					{
						if (nx_stair > 0)
							whip->SetPosition(r, t);
						else
							whip->SetPosition(l - 24, t);
					}
					whip->Render(1, nx_stair, currentframe, alpha);
				}
			}
		}

		if (isColor == true)
		{
			if (nx > 0)
				ani = SIMON_ANI_COLORS_RIGHT;
			else
				ani = SIMON_ANI_COLORS_LEFT;
			int currentframe = animation_set->at(ani)->GetcurrenFrame();
			if (loopani>1) 
			{
				isColor = false;
				if (nx > 0)
					ani = SIMON_ANI_IDLE_RIGHT;
				else
					ani = SIMON_ANI_IDLE_LEFT;
			}
			if (currentframe == 0 && preframe == animation_set->at(ani)->size()-1)
			{
				loopani++;
			}

			preframe = currentframe;
			
		}
		if (isJump == true)
		{
			if (nx > 0) ani = SIMON_ANI_JUMP_RIGHT;
			if (nx < 0) ani = SIMON_ANI_JUMP_LEFT;
		}
		if (isStanding == true)
		{
			if (nx > 0)
				ani = SIMON_ANI_STANDING_RIGHT;
			else
				ani = SIMON_ANI_STANDING_LEFT;

			int currentframe = animation_set->at(ani)->GetcurrenFrame();
			if (currentframe > 2)
			{
				isStanding = false;
				whip->SetPosition(-10, -10);
				animation_set->at(ani)->SetCurrenFrame(-1);
				if (nx > 0)
					ani = SIMON_ANI_IDLE_RIGHT;
				else
					ani = SIMON_ANI_IDLE_LEFT;
			}
			else
			{
				float l, t, r, b;
				GetBoundingBox(l, t, r, b);
				if (nx > 0)
					whip->SetPosition(l-10, t);
				else
					whip->SetPosition(r, t);
				if (currentframe == 2)
				{
					if (nx > 0)
						whip->SetPosition(r, t);
					else
						whip->SetPosition(l - 24, t);
				}
				whip->Render(1, nx, currentframe, alpha);
			}
		}
		
		//DebugOut(L"Xuat isStanding: %d curentframe: %d\n", isStanding, animation_set->at(ani)->GetcurrenFrame());
		if (ani == SIMON_ANI_ASCEND_WHIP_RIGHT && animation_set->at(ani)->GetcurrenFrame() == 0)
			animation_set->at(ani)->Render(x - 8, y, alpha);
		else
			animation_set->at(ani)->Render(x, y, alpha);
	
	//RenderBoundingBox();
	//DebugOut(L"Xuat isJump: %d vy= %d\n", isJump, vy);
	//DebugOut(L"Xuat curenframe: %d\n", animation_set->at(ani)->GetcurrenFrame());

}

void Simon::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_IDLE_RIGHT:
		vx = 0;
		nx = 1;
		break;
	case SIMON_STATE_IDLE_LEFT:
		vx = 0;
		nx = -1;
		break;
	case SIMON_STATE_WALK_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_WALK_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_JUMP_LEFT:
		vy = -SIMON_JUMP_SPEED_Y;
		nx = -1;
		isJump = true;
		break;
	case SIMON_STATE_JUMP_RIGHT:
		vy = -SIMON_JUMP_SPEED_Y;
		nx = 1;
		isJump = true;
		break;
	case SIMON_STATE_ASCEND_LEFT:
		vx=vy = 0;
		isOnStair = true;
		break;
	case SIMON_STATE_ASCEND_RIGHT:
		vx=vy = 0;
		isOnStair = true;
		isIdleOnStair = false;
		break;
	case SIMON_STATE_DESCEND_LEFT:
		vx = vy = 0;
		isOnStair = true;
		break;
	case SIMON_STATE_DESCEND_RIGHT:
		vx = SIMON_WALKING_SPEED;
		vy = -SIMON_WALKING_SPEED;
		isOnStair = true;
		break;
	case SIMON_STATE_HURT_LEFT:
		vx = 0;
		nx = -1;
		break;
	case SIMON_STATE_HURT_RIGHT:
		vx = 0;
		nx = 1;
		break;
	case SIMON_STATE_DEATH_LEFT:
		vx = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_DEATH_RIGHT:
		vx = SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_INTRO:
		vx = 0;
	case SIMON_STATE_STANDING_LEFT:
		vx = 0;
		nx = -1;
		isStanding = true;
		break;
	case SIMON_STATE_STANDING_RIGHT:
		vx =0;
		nx = 1;
		isStanding = true;
		break;
	case SIMON_STATE_COLORS_RIGHT:
		vx = vy = 0;
		nx = 1;
		isColor = true;
		break;
	case SIMON_STATE_COLORS_LEFT:
		vx = vy = 0;
		nx = -1;
		isColor = true;
		break;
			
	}
	
}

void Simon::Idle()
{
	if (nx > 0)
		SetState(SIMON_STATE_IDLE_RIGHT);
	else
		SetState(SIMON_STATE_IDLE_LEFT);
	isOnStair = false;
	isDownStair = false;
	isUpStair = false;
}

void Simon::Jump()
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	if (vy<0.1)
	{
		if (nx > 0)
			SetState(SIMON_STATE_JUMP_RIGHT);
		else
			SetState(SIMON_STATE_JUMP_LEFT);
	}
}

void Simon::Standing()
{
	if (isOnStair)
		isWhip = true;
	else
	{
			if (nx > 0)
				SetState(SIMON_STATE_STANDING_RIGHT);
			else
				SetState(SIMON_STATE_STANDING_LEFT);
	}
}

void Simon::Color()
{
	if (nx > 0)
		SetState(SIMON_STATE_COLORS_RIGHT);
	else
		SetState(SIMON_STATE_COLORS_LEFT);
	isStanding = false;
	isJump = false;
}

void Simon::Reset()
{
	SetState(SIMON_STATE_IDLE_RIGHT);
	SetLevel(1);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void Simon::StairDown()
{
	/*if (!isDownStair)
	{
		if (isUpStair)
			nx_stair = -direcStair;
		else
			nx_stair = direcStair;
	}*/
	nx_stair = -direcStair;
	isOnStair = true;
	isUpStair = false;
	isDownStair = true;
	isIdleOnStair = false;
	if (distanceX == 0 && distanceY == 0)
	{
		distanceX = distanceY = 8;
	}
}

void Simon::StairUp()
{
	//if (!isUpStair)
		//if (isDownStair)
			//nx_stair = -direcStair;
		//else
			//nx_stair = direcStair;
	nx_stair = direcStair;
	isOnStair = true;
	isUpStair = true;
	isDownStair = false;
	isIdleOnStair = false;
	if (distanceX == 0 && distanceY == 0)
	{
		distanceX = distanceY = 8;
	}

}

void Simon::IdleOnStair()
{
	isIdleOnStair = true;
	vx = vy = 0;
	DebugOut(L"dung \n");
}

void Simon::SetWhip(Whip* whiptemp)
{
	whip = whiptemp;
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (state)
	{
	case SIMON_STATE_IDLE_LEFT:
	case SIMON_STATE_IDLE_RIGHT:
		right = x + 16;
		bottom = y + 30;
		break;
	case SIMON_STATE_JUMP_RIGHT:
	case SIMON_STATE_JUMP_LEFT:
		right = x + 16;
		bottom = y + 30;
		break;
	case SIMON_STATE_WALK_LEFT:
	case SIMON_STATE_WALK_RIGHT:
		right = x + 16;
		bottom = y + 30;
		break;
	case SIMON_STATE_STANDING_LEFT:
	case SIMON_STATE_STANDING_RIGHT:
		right = x + 24;
		bottom=y+30;
		break;
	case SIMON_STATE_COLORS_LEFT:
	case SIMON_STATE_COLORS_RIGHT:
		right = x + 16;
		bottom = y + 30;
		break;
	default:
		right = x + 16;
		bottom = y + 32;
		break;
	}
	if (isOnStair)
	{
		right = x + 16;
		bottom = y + 32;
	}

}

void Simon::Unload()
{
	__instance = NULL;
}

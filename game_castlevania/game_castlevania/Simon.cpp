#include "Simon.h"
#include "Utils.h"
#include "Brick.h"
#include "Torch.h"

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

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	//if (vy == 0) isJump = false;

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
	
	if (coEvents.size() == 0)
	{
		x += dx;
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
			//isJump = false;
		}
		//else
			//isJump = true;

		 //Collision logic with brick
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj)) // if e->obj is Goomba 
				isJump = false;
		}
		// Collision logic with Goombas
		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	LPCOLLISIONEVENT e = coEventsResult[i];

		//	if (dynamic_cast<Goomba*>(e->obj)) // if e->obj is Goomba 
		//	{
		//		Goomba* goomba = dynamic_cast<Goomba*>(e->obj);

		//		// jump on top >> kill Goomba and deflect a bit 
		//		if (e->ny < 0)
		//		{
		//			if (goomba->GetState() != GOOMBA_STATE_DIE)
		//			{
		//				goomba->SetState(GOOMBA_STATE_DIE);
		//				vy = -MARIO_JUMP_DEFLECT_SPEED;
		//			}
		//		}
		//		else if (e->nx != 0)
		//		{
		//			if (untouchable == 0)
		//			{
		//				if (goomba->GetState() != GOOMBA_STATE_DIE)
		//				{
		//					if (level > MARIO_LEVEL_SMALL)
		//					{
		//						level = MARIO_LEVEL_SMALL;
		//						StartUntouchable();
		//					}
		//					else
		//						SetState(MARIO_STATE_DIE);
		//				}
		//			}
		//		}
		//	}
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
		case SIMON_STATE_JUMP_RIGHT:
			ani = SIMON_ANI_JUMP_RIGHT;
			isJump = true;
			break;
		case SIMON_STATE_JUMP_LEFT:
			ani = SIMON_ANI_JUMP_LEFT;
			isJump = true;
			break;
		case SIMON_STATE_ASCEND_LEFT:
			ani = SIMON_ANI_ASCEND_LEFT;
			break;
		case SIMON_STATE_ASCEND_RIGHT:
			ani = SIMON_ANI_ASCEND_RIGHT;
			break;
		case SIMON_STATE_DESCEND_LEFT:
			ani = SIMON_ANI_DESCEND_LEFT;
			break;
		case SIMON_STATE_HURT_LEFT:
			ani = SIMON_ANI_HURT_LEFT;
			break;
		case SIMON_STATE_HURT_RIGHT:
			ani = SIMON_ANI_HURT_RIGHT;
			break;
		case SIMON_STATE_DEATH_LEFT:
			ani = SIMON_ANI_DEATH_LEFT;
			break;
		case SIMON_STATE_DEATH_RIGHT:
			ani = SIMON_ANI_DEATH_RIGHT;
			break;
		case SIMON_STATE_INTRO:
			ani = SIMON_ANI_INTRO;
			break;
		case SIMON_STATE_COLORS_LEFT:
			ani = SIMON_ANI_COLORS_LEFT;
			break;
		case SIMON_STATE_COLORS_RIGHT:
			ani = SIMON_ANI_COLORS_RIGHT;
			break;
		case SIMON_STATE_STANDING_LEFT:
			ani = SIMON_ANI_STANDING_LEFT;
			break;
		default:
			break;
		}

		/*if (isJump == true)
		{
			if (nx > 0) ani = SIMON_ANI_JUMP_RIGHT;
			if (nx < 0) ani = SIMON_ANI_JUMP_LEFT;
		}*/
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
				//whip->SetPosition(0, 0);
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
		DebugOut(L"Xuat isStanding: %d curentframe: %d\n", isStanding, animation_set->at(ani)->GetcurrenFrame());
	animation_set->at(ani)->Render(x, y, alpha);
	
	

	RenderBoundingBox();
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
	/*case SIMON_STATE_JUMP_LEFT:
		vy = -SIMON_JUMP_SPEED_Y;
		nx = -1;
		isJump = true;
		break;
	case SIMON_STATE_JUMP_RIGHT:
		vy = -SIMON_JUMP_SPEED_Y;
		nx = 1;
		break;*/
	case SIMON_STATE_ASCEND_LEFT:
		vx = -1.5 * SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_ASCEND_RIGHT:
		vx = 1.5 * SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_DESCEND_LEFT:
		vx = -0.5 * SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_DESCEND_RIGHT:
		vx = 0.5 * SIMON_WALKING_SPEED;
		nx = 1;
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
		vx = vy = 0;
		nx = -1;
		isStanding = true;
		break;
	case SIMON_STATE_STANDING_RIGHT:
		vx = vy = 0;
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
}

void Simon::Jump()
{
	if (nx > 0)
		SetState(SIMON_STATE_JUMP_RIGHT);
	else
		SetState(SIMON_STATE_JUMP_LEFT);
}

void Simon::Standing()
{
	if (nx > 0)
		SetState(SIMON_STATE_STANDING_RIGHT);
	else
		SetState(SIMON_STATE_STANDING_LEFT);
}

void Simon::Color()
{
	if (nx > 0)
		SetState(SIMON_STATE_COLORS_RIGHT);
	else
		SetState(SIMON_STATE_COLORS_LEFT);
}

void Simon::Reset()
{
	SetState(SIMON_STATE_IDLE_RIGHT);
	SetLevel(1);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
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
		break;
	}

}

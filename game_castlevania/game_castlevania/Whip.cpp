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
	vy += 0.002*dt;
	vx += 0.002*dt;;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);
	//DebugOut(L"coevents.size=: %d\n", coEvents.size());
	//DebugOut(L"x:%f y:%f\n", x,y);
	//DebugOut(L"torch->animation_set->at(0)->GetcurrenFrame()==%d\n", torch->animation_set->at(0)->GetcurrenFrame());

	if (coEvents.size() == 0)
	{
	}
	else
	{
		float min_tx, min_ty, nxa = 0, nya;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nxa, nya);

		int sumtorch = -1;
		 //Collision logic with brick
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Torch*>(e->obj))
			{
				//DebugOut(L"Co va cham Torch\n");
				sumtorch++;
				//delete e->obj
				Torch* torch = dynamic_cast<Torch*>(e->obj);
				DebugOut(L"which->animation_set->at(0)->GetcurrenFrame()==%d, nx=%d\n", animation_set->at(0)->GetcurrenFrame(),nx);
				DebugOut(L"which->animation_set->at(1)->GetcurrenFrame()==%d, nx=%d\n",animation_set->at(1)->GetcurrenFrame(),nx);
				
				if (nx < 0 && animation_set->at(0)->GetcurrenFrame() == 2)torch->SetColi(true);
				if (nx > 0 && animation_set->at(1)->GetcurrenFrame() == 2)torch->SetColi(true);
				//if (animation_set->at(1)->GetcurrenFrame() == 2)torch->SetColi(true);
				//if (animation_set->at(0)->GetcurrenFrame() == 2)torch->SetColi(true);

			}

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
		{
			animation_set->at(1)->SetCurrenFrame(frame);
			animation_set->at(1)->RenderFrame(x, y, frame, alpha);
		}
		else
		{
			animation_set->at(0)->SetCurrenFrame(frame);
			animation_set->at(0)->RenderFrame(x, y, frame, alpha);
		}
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
	r = x + 12;
	b = y + 16;
}
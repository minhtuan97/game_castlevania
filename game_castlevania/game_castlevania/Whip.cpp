#include "Whip.h"
#include "Candle.h"
#include "Grid.h"


void Whip::Render()
{
	animation_set->at(1)->Render(x, y);
	//RenderBoundingBox();
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{

	GameObject::Update(dt);
	for (UINT i = 0; i < colliable_objects->size(); i++)
	{
		float l1, t1, r1, b1, l2, t2, r2, b2;
		GetBoundingBox(l1, t1, r1, b1);
		colliable_objects->at(i)->GetBoundingBox(l2, t2, r2, b2);
		//if (dynamic_cast<Torch*>(colliable_objects->at(i)))
			//DebugOut(L"whip: ml:%f,mt:%f,mr:%f,\mb:%f torch: l:%f,t:%f,r:%f,b:%f \n", l1, t1, r1, b1, l2, t2, r2, b2);
		if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
		{
			if (dynamic_cast<Torch*>(colliable_objects->at(i)))
			{
				Torch* torch = dynamic_cast<Torch*>(colliable_objects->at(i));
				if (animation_set->at(0)->GetcurrenFrame() == 2)torch->SetColi(true);
				if (animation_set->at(1)->GetcurrenFrame() == 2)torch->SetColi(true);
				//DebugOut(L"Co va cham voi lua tai vi tri x:%f,y%f\n",torch->x,torch->y);

			}
			else
				if (dynamic_cast<Candle*>(colliable_objects->at(i)))
				{
					Candle* candle = dynamic_cast<Candle*>(colliable_objects->at(i));
					if (animation_set->at(0)->GetcurrenFrame() == 2)candle->SetColi(true);
					if (animation_set->at(1)->GetcurrenFrame() == 2)candle->SetColi(true);
					//DebugOut(L"Co va cham voi lua tai vi tri x:%f,y%f\n",torch->x,torch->y);

				}
		}

	}
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
	r = x + 24;
	b = y + 16;
}
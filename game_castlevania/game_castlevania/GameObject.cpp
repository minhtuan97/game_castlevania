#include "GameObject.h"
#include "Game.h"
#include <typeinfo>
#include "Torch.h"
#include "Utils.h"


GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}



void GameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

LPCOLLISIONEVENT GameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);
	
	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;


	GetBoundingBox(ml, mt, mr, mb);

	Game::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
		);

	CollisionEvent* e = new CollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}
void GameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		//if(dynamic_cast<Torch*>(coObjects->at(i)))
			//DebugOut(L"Object Torch\n");

		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		//DebugOut(L"xuat t: %f \n", e->t);
		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);//lấy t kiểm tra va chạm
			//if(dynamic_cast<Torch*>(e->obj))
			//DebugOut(L"co va cham Object Torch, vi tri roi: %f %f\n",x,y);
		}
		else
			delete e;// xóa e đi nếu t ko va chạm
	}

	/*if (coEvents.size() !=0)
	{
		DebugOut(L"co va cham: \n");
	}
	else
	{
		DebugOut(L"ko co va cham: \n");
	}*/

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}

void GameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void GameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	Game::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 111);
}


GameObject::~GameObject()
{
}

#include "Brickmove.h"
#include "Utils.h"

Brickmove::Brickmove()
{
	vx = speed;
}

void Brickmove::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt, coObjects);
	x += dx;
	if (vx > 0 && x >= x_max)
		vx = -speed;
	if (vx < 0 && x <= x_min)
		vx = speed;
	//DebugOut(L"Xuat xmin=%f, xmax=%f, x=%f, y=%f, vx=%f\n", x_min, x_max, x, y, vx);
}

void Brickmove::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Brickmove::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 32;
	b = y + 8;
}

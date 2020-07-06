#include "Knight.h"
#include"Brick.h"

Knight::Knight(float x, float y)
{
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	vx = 0.02f;
	vy = 0;
	state = 1;
}

void Knight::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	GameObject::Update(dt, colliable_objects);
	x += dx;
	y += dy;

	if (vx < 0 && x < left_max) {
		x = left_max; vx = -vx;
		state = 1;
	}

	if (vx > 0 && x > right_max) {
		x = right_max; vx = -vx;
		state = 0;
	}
}

void Knight::Render()
{
	animation_set->at(state)->Render(x, y);
	//RenderBoundingBox();
}

void Knight::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 16;
	bottom = top + 32;
}

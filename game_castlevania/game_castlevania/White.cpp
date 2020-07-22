#include "White.h"

White::White()
{
}

void White::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void White::Render()
{
	int ani = 1;
	animation_set->at(ani)->Render(x, y);
}

void White::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void White::SetState(int state)
{
}

White::~White()
{
}

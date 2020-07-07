#include "StairBot.h"

StairBot::StairBot()
{
}

void StairBot::Render()
{
	RenderBoundingBox();
}

void StairBot::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;

}

StairBot::~StairBot()
{
}

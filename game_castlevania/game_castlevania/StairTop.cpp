#include "StairTop.h"

StairTop::StairTop()
{
}

void StairTop::Render()
{
	RenderBoundingBox();
}

void StairTop::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}

StairTop::~StairTop()
{
}

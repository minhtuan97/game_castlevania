#include "BrickJump.h"

void BrickJump::Render()
{
	//RenderBoundingBox();
}

void BrickJump::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 8;
	bottom = y + 8;
}

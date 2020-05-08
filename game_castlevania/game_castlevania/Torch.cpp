#include "Torch.h"

void Torch::Render()
{
	animation_set->at(0)->Render(x,y);
}

void Torch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + 16;
	b = t + 32;
}

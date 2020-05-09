#include "Torch.h"

void Torch::Render()
{
	if (isColi == false)
	{
		animation_set->at(0)->Render(x, y);
	}
	else
	{
		SetPosition(-1, -1);

	}
}

void Torch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + 16;
	b = t + 32;
}

void Torch::SetColi(bool t)
{
	isColi = t;
}

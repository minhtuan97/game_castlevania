#include "Torch.h"
#include "Grid.h"
#include "Item.h"

void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* listItem)
{
	if (isColi)
	{
		Grid* grid = Grid::GetInstance();
		for (int i = 0; i < listItem->size(); i++)
		{
				Item* item = dynamic_cast<Item*>(listItem->at(i));
				if(item->ID==ID_Item)
					grid->addObject(item);
		}
	}
}

void Torch::Render()
{
		animation_set->at(0)->Render(x, y);
		RenderBoundingBox();
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

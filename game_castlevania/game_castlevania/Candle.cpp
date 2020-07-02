#include "Candle.h"
#include "Grid.h"
#include "Item.h"

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* listItem)
{
	if (isColi)
	{
		Grid* grid = Grid::GetInstance();
		for (int i = 0; i < listItem->size(); i++)
		{
			Item* item = dynamic_cast<Item*>(listItem->at(i));
			if (item->ID == ID_Item)
				grid->addObject(item);
		}
	}
}

void Candle::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Candle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + 8;
	b = t + 16;
}

void Candle::SetColi(bool t)
{
	isColi = t;
}

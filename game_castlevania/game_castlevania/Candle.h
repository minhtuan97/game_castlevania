#pragma once
#include "GameObject.h"

class Candle:public GameObject
{
public:
	int ID_Item;
	bool isColi = false;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listItem);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetColi(bool t);
};


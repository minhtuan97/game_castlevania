#pragma once
#include "GameObject.h"
class Item: public GameObject
{
	int type;
public:
	bool isEnable;
	bool isfinish;
	int x_de, y_de;
	Item();
	//Item(int typeItem);
	~Item();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	int GetTypeItem() { return type; }
	void Settype(int t) { type = t; }
	//void TurnOnTimeStartEnable();

};


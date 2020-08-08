#pragma once
#include "GameObject.h"

class CNumber: public GameObject
{
	public:
		int num;
		CNumber();
		CNumber(int number);
		virtual void Render();
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
		vector<int> Tachso(int x);
};


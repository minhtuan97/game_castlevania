#include "CNumber.h"

CNumber::CNumber()
{
}

CNumber::CNumber(int number)
{
	num = number;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATIONSET_NUMBER);
	SetAnimationSet(ani_set);
}

void CNumber::Render()
{
	Camera* cam = Camera::GetInstance();
	vector<int> tachso = Tachso(num);
	for (int i = 0; i < tachso.size(); i++)
	{
		animation_set->at(tachso.at(i))->Render(208 + i * 9+cam->GetCameraPosition().x, 16);
	}
}

void CNumber::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

vector<int> CNumber::Tachso(int x)
{
	vector<int> a;
	int i = 1;
	while (x / 10 >= 1)
	{
		a.push_back(x % 10);
		i++;
		x = x / 10;
	}
	a.push_back(x % 10);

	vector<int> b;
	if (i == 1)
		b.push_back(0);
	for (int k = i-1; k >= 0; k--)
	{
		b.push_back(a.at(k));
	}
	return b;
}

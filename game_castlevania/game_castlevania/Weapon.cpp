#include "Weapon.h"

Weapon::Weapon()
{
		isAttack = false;
		aniFreeze = 0;
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Weapon::Render()
{
}

void Weapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void Weapon::CollisionWithListObjects(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isAttack)return;
}

void Weapon::setPosition(float _x, float _y, int _nx)
{
	GameObject::SetPosition(_x, _y);
	this->nx = _nx;
	timeAttact = GetTickCount();
}

void Weapon::SetFirst(bool _isFirst)
{
}

bool Weapon::GetFirst()
{
	return false;
}

void Weapon::Reset(float x_t, float y_t, int nx_temp)
{

}


Weapon::~Weapon()
{
}

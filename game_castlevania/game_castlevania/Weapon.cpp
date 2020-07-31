#include "Weapon.h"
#include "Bat.h"
#include "Grid.h"
#include "Ghost.h"
#include "Knight.h"
#include "Monkey.h"
#include "Zombie.h"
#include "BatBoss.h"
#include "Candle.h"

Weapon::Weapon()
{
		isAttack = false;
		aniFreeze = 0;
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Bat*>(coObjects->at(i)))
		{
			Bat* b = dynamic_cast<Bat*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			b->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				Grid* grid = Grid::GetInstance();
				grid->deleteObject(b);
			}
		}
		if (dynamic_cast<Ghost*>(coObjects->at(i)))
		{
			Ghost* b = dynamic_cast<Ghost*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			b->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				Grid* grid = Grid::GetInstance();
				grid->deleteObject(b);
			}
		}
		if (dynamic_cast<Knight*>(coObjects->at(i)))
		{
			Knight* b = dynamic_cast<Knight*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			b->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				Grid* grid = Grid::GetInstance();
				grid->deleteObject(b);
			}
		}
		if (dynamic_cast<Monkey*>(coObjects->at(i)))
		{
			Monkey* b = dynamic_cast<Monkey*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			b->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				Grid* grid = Grid::GetInstance();
				grid->deleteObject(b);
			}
		}
		if (dynamic_cast<Zombie*>(coObjects->at(i)))
		{
			Zombie* b = dynamic_cast<Zombie*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			b->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				Grid* grid = Grid::GetInstance();
				grid->deleteObject(b);
			}
		}
		if (dynamic_cast<BatBoss*>(coObjects->at(i)))
		{
			BatBoss* b = dynamic_cast<BatBoss*>(coObjects->at(i));

			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			b->GetBoundingBox(l2, t2, r2, b2);

			if (Game::AABB(l1, t1, r1, b1, l2, t2, r2, b2))
			{
				b->SubHeart();
			}
		}
		if (dynamic_cast<Candle*>(coObjects->at(i)))
		{
			Candle* candle = dynamic_cast<Candle*>(coObjects->at(i));
			candle->SetColi(true);

		}
	}
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

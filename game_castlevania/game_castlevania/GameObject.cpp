#include "GameObject.h"
#include "debug.h"
#include "Game.h"

vector<LPANIMATION> GameObject::animations;

void GameObject::AddAnimation(int aniId)
{
	LPANIMATION ani = Animations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}

GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void GameObject::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void GameObject::Render()
{
	//LPANIMATION ani;
	//if (vx > 0) ani = animations[0]; else ani = animations[1];
	////ani = animations[2];
	//ani->Render(x, y);
}

GameObject::~GameObject()
{
}

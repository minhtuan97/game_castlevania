#include "Animations.h"

void Animations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION Animations::Get(int id)
{
	return animations[id];
}

Animations* Animations::_instance = NULL;

Animations* Animations::GetInstance()
{
	if (_instance == NULL) _instance = new Animations();
	return _instance;
}

#pragma once
#include <d3dx9.h>
#include "Sprite.h"
#include <unordered_map>

using namespace std;

/*
	Manage sprite database
*/
class Sprites
{
	static Sprites* _instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);
	LPSPRITE& operator[](int id) { return sprites[id]; }

	void Clear();
	static Sprites* GetInstance();
};


#pragma once
#include <d3dx9.h>
#include <unordered_map>
#include "Game.h"

class Sprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, int alpha =255);
};

typedef Sprite* LPSPRITE;
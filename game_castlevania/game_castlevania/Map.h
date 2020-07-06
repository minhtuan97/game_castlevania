#pragma once
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"
#include <fstream> 
#include <string>
#include <stdio.h>
#include <Windows.h>  
#include <iostream>
#include "Animation.h"

#define ID_TEX_MAP1 50
class Map
{
	int ColumnTile=0;
	int RowTile=0;
	int TotalTiles=0;
	int widthTile=0, heightTile=0;
	int ColumnMap=0, RowMap=0;
	int TileMap[500][500];
	static Map* _instance;
public:
	Map();
	void LoadFile(LPCWSTR filename);
	void LoadMap(int idTextureMap);
	void DrawMap();
	void Clear();
	float GetHeight();
	float GetWidth();
	static Map* GetInstance();

	~Map();
};


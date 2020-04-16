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
	Textures* _texture;
	Sprites* _sprites;
	int ColumnTile;
	int RowTile;
	int TotalTiles;
	int widthTile, heightTile;
	int ColumnMap, RowMap;
	int TileMap[500][500];

public:
	Map(LPCWSTR filename);
	void LoadMap(int idTextureMap);
	//void ReadMapTXT(char* filename);
	void DrawMap();
	//int GetMapWidth();
	//int GetMapHeight();

	~Map();
};

#include "Map.h"
#include "Utils.h"

Map::Map(LPCWSTR filename)
{
	ifstream inp(filename, ios::in);
	inp >> TotalTiles >> RowMap >> ColumnMap >> RowTile >> ColumnTile;
	for (int i = 0; i < RowMap; i++)
		for (int j = 0; j < ColumnMap; j++)
		{
			inp >> TileMap[i][j];
			//DebugOut(L"xuat : %d", TileMap[i][j]);
		}
	inp.close();
	DebugOut(L"xuat : %d %d %d %d %d \n", TotalTiles, RowMap, ColumnMap, RowTile, ColumnTile);

	widthTile = heightTile = 64;
}

void Map::LoadMap(int idTextureMap)
{
	Textures* textures = Textures::GetInstance();

	Sprites* sprites = Sprites::GetInstance();

	LPDIRECT3DTEXTURE9 texMap1 = textures->Get(idTextureMap);
	for (int i = 0; i < RowMap; i++)
		for (int j = 0; j < ColumnMap; j++)
		{
			int a = TileMap[i][j];

			sprites->Add(50000 + i * ColumnMap + j, a * widthTile, 0, (a + 1) * widthTile, 64, texMap1);
		}
}

void Map::DrawMap()
{
	Sprites* sprites = Sprites::GetInstance();
	for (int i = 0; i < RowMap; i++)
		for (int j = 0; j < ColumnMap; j++)
		{
			sprites->Get(50000 + i * ColumnMap + j)->Draw(j * widthTile, i * heightTile);
		}		
}

Map::~Map()
{
}

#include "Map.h"
#include "Utils.h"

Map::Map(LPCWSTR filename)
{
	ifstream inp(filename, ios::in);
	inp >> TotalTiles >> RowMap >> ColumnMap >> RowTile >> ColumnTile >> widthTile >> heightTile;
	for (int i = 0; i < RowMap; i++)
		for (int j = 0; j < ColumnMap; j++)
		{
			inp >> TileMap[i][j];
			//DebugOut(L"xuat : %d", TileMap[i][j]);
		}
	inp.close();
	DebugOut(L"xuat : %d %d %d %d %d \n", TotalTiles, RowMap, ColumnMap, RowTile, ColumnTile);
}

void Map::LoadMap(int idTextureMap)
{
	Textures* textures = Textures::GetInstance();

	Sprites* sprites = Sprites::GetInstance();

	LPDIRECT3DTEXTURE9 texMap1 = textures->Get(idTextureMap);

	for (int i = 0; i < RowTile; i++)
		for (int j = 0; j < ColumnTile; j++)
		{
			sprites->Add(50000 + i * ColumnTile + j, j * widthTile, i*heightTile, (j + 1) * widthTile, (i+1) * heightTile, texMap1);
		}
}

void Map::DrawMap()
{
	Sprites* sprites = Sprites::GetInstance();
	int a = -1;
	for (int i = 0; i < RowMap; i++)
		for (int j = 0; j < ColumnMap; j++)
		{
			a = TileMap[i][j];
			sprites->Get(50000 +a)->Draw(j * widthTile, i * heightTile);
		}		
}

Map::~Map()
{
}

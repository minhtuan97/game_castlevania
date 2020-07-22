#include "Map.h"
#include "Utils.h"
#include "Camera.h"
#include "define.h"

Map* Map::_instance = NULL;

Map::Map()
{
}

void Map::LoadFile(LPCWSTR filename)
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
	Camera* cam = Camera::GetInstance();
	int tile_column = (int) SCREEN_WIDTH / widthTile+2;
	int tile_row = RowTile;
	float mx, my;
	mx = cam->GetCameraPosition().x;
	//my = cam->GetCameraPosition().y;
	int tile_start_column = (int)mx / widthTile;
	//int tile_start_row = (int)my / widthTile;
	Sprites* sprites = Sprites::GetInstance();
	int a = -1;
	//DebugOut(L"bat dau\n");
	for (int i = 0; i < RowMap; i++)
		for (int j = tile_start_column; j < tile_column+ tile_start_column; j++)
		//for (int j = 0; j < ColumnMap; j++)
		{
			
			a = TileMap[i][j];
			if(a>=0)
				sprites->Get(50000 + a)->Draw(j * widthTile, i * heightTile);
			//if (a == 55)DebugOut(L"%d	%d\n", j * widthTile, i * heightTile);
		}
	//DebugOut(L"bhet\n");

}

void Map::Clear()
{
	if (_instance == NULL)
		return;
	else
	{
		/*for (int i = 0; i < RowMap; i++)
			delete[] TileMap[i];
		delete[] TileMap;*/
		_instance = NULL;
	}
}

float Map::GetHeight()
{
	return heightTile* (RowMap);
}

float Map::GetWidth()
{
	return widthTile * (ColumnMap);
}

Map* Map::GetInstance()
{
	if (_instance == NULL) _instance = new Map();
	return _instance;
}

Map::~Map()
{
}

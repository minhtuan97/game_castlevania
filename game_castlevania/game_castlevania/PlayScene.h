#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Map.h"
#include "Simon.h"
#include "Torch.h"
#include "Whip.h"
#include "Camera.h"
#include "Grid.h"
#include "Item.h"

class CPlayScene : public CScene
{
protected:
	Map* map;
	Grid* grid;
	//Mario* player;					// A play scene has to have player, right? 
	Simon* player2;
	Camera* camera;
	Whip* whip;
	vector<LPGAMEOBJECT> objects;
	
	vector<LPGAMEOBJECT> objects_item;
	float xLeftCreateGhost, xRightCreateGhost;
	float yCreateGhost;
	boolean isCreateGhost = false;
	int animationsetID;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);


public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//Mario* GetPlayer() { return player; }
	Simon* GetPlayer2() { return player2; }
	void GetWhip();
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};


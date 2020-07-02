
#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Torch.h"
#include "define.h"
#include "StairTop.h"
#include "StairBot.h"
#include "Bat.h"
#include "Knight.h"
#include "Candle.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	map = Map::GetInstance();
	camera = Camera::GetInstance();
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7


#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	Textures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	Sprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	Animations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	Animations* animations = Animations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y

	int ID = atoi(tokens[0].c_str());
	int object_type = atoi(tokens[1].c_str());
	float x = atof(tokens[2].c_str());
	float y = atof(tokens[3].c_str());

	int ani_set_id = atoi(tokens[4].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	GameObject* obj = NULL;

	switch (object_type)
	{
	case 4:
		if (player2 != NULL)
		{
			DebugOut(L"[ERROR] PLAYER2 object was created before!\n");
			return;
		}
		//obj = new Simon(x, y);
		obj = Simon::GetInstance();
		//player2 = Simon::GetInstance();
		player2 = (Simon*)obj;
		DebugOut(L"[INFO] Player2 object created!\n");
		break;
	case 2: 
		obj = new Torch(); 
		Torch* torch;
		torch = NULL;
		torch = (Torch*)obj;
		torch->ID_Item= (int)atof(tokens[5].c_str());
		break;
	case OBJECT_TYPE_BRICK: 
		obj = new Brick(); break;
	case 3: 
		obj = new Whip();
		whip = (Whip*)obj;
		player2->SetWhip(whip);
		break;
	case 5:
	{
		obj = new Item();
		Item* item = NULL;
		item = (Item*)obj;
		item->Settype((int)atof(tokens[5].c_str()));
		obj->SetPosition(x, y);
		obj->ID = ID;
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		objects_item.push_back(obj);
		if(item->GetTypeItem()!=3)
			return;
		break;
	}
	case 6:
	{
		obj = new StairTop();
		StairTop* item = NULL;
		item = (StairTop*)obj;
		item->nx=(int)atof(tokens[5].c_str());
		break;
	}
	case 7:
	{
		obj = new StairBot();
		StairBot* item = NULL;
		item = (StairBot*)obj;
		item->nx = (int)atof(tokens[5].c_str());
		
		break;
	}
	case 8:
	{
		obj = new Bat(x,y);
		float l = atof(tokens[5].c_str());
		float r = atof(tokens[6].c_str());
		float t = atof(tokens[7].c_str());
		float b = atof(tokens[8].c_str());
		Bat* bat = (Bat*)obj;
		bat->left_max = l;
		bat->right_max = r;
		bat->top_max = t;
		bat->bot_max = b;
		break;
	}
	case 9:
	{
		obj = new Knight(x, y);
		float l = atof(tokens[5].c_str());
		float r = atof(tokens[6].c_str());
		Knight* knight = (Knight*)obj;
		knight->left_max = l;
		knight->right_max = r;
		break;
	}
	case 10:
		obj = new Candle();
		Candle* candle;
		candle = NULL;
		candle = (Candle*)obj;
		candle->ID_Item = (int)atof(tokens[5].c_str());
		break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		int scene_id = atoi(tokens[7].c_str());
		obj = new Portal(x, y, r, b, scene_id);
		break;
	}	
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	obj->ID = ID;
	if (ani_set_id != -1)
	{
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
	}
	objects.push_back(obj);
	grid = Grid::GetInstance();
	if(obj!=NULL&& !dynamic_cast<Simon*>(obj))
		grid->addObject(obj);
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	LPCWSTR pathMap = ToLPCWSTR(tokens[1].c_str());
	map->LoadFile(pathMap);
	map->LoadMap(atoi(tokens[0].c_str()));
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;

		}
	}

	f.close();

	Textures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	
	grid->GetListOfObjects(&objects);
	
	vector<LPGAMEOBJECT> coObjects2;
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> listTorch;
	vector<LPGAMEOBJECT> listCandle;
	vector<LPGAMEOBJECT> listItem;

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Torch*>(objects[i]))
			listTorch.push_back(objects[i]);
		else
			if (dynamic_cast<Candle*>(objects[i]))
				listCandle.push_back(objects[i]);
			else
			{
				coObjects.push_back(objects[i]);
			if (dynamic_cast<Item*>(objects[i]))
				listItem.push_back(objects[i]);
			else
				{
				coObjects2.push_back(objects[i]);
				}
			}
	}
	player2->Update(dt, &coObjects);
		
	whip->Update(dt, &listTorch);
	whip->Update(dt, &listCandle);

	for (int i = 0; i < listTorch.size(); i++)
	{
		Torch* a = dynamic_cast<Torch*>(listTorch[i]);
		a->Update(dt, &objects_item);
		/*if(a->isColi)
		{
			listTorch[i]->Update(dt, &objects_item);
		}*/
	}
	for (int i = 0; i < listCandle.size(); i++)
	{
		Candle* a = dynamic_cast<Candle*>(listCandle[i]);
		a->Update(dt, &objects_item);
		/*if (a->isColi)
		{
			listCandle[i]->Update(dt, &objects_item);
		}*/
	}

	for (size_t i = 0; i < coObjects2.size(); i++)
	{
		coObjects2[i]->Update(dt, &coObjects2);
	}

	for (size_t i = 0; i < listItem.size(); i++)
	{
		listItem[i]->Update(dt, &coObjects2);
	}


	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player2 == NULL) return;

	// Update camera to follow mario
	float cx, cy, mapheight, mapwidth;
	mapheight = map->GetHeight();
	mapwidth = map->GetWidth();
	player2->GetPosition(cx, cy);
	//DebugOut(L"vi tri simon: %f,%f\n", cx,cy);

	//Game* game = Game::GetInstance();
	//cx -= game->GetScreenWidth() / 2;
	//cy -= game->GetScreenHeight() / 2;
	D3DXVECTOR3 pos = camera->GetCameraPosition();
	if (mapwidth > SCREEN_WIDTH ) {
		if (cx  + 5 < SCREEN_WIDTH / 2) {
			cx = pos.x;
		}
		else if (cx + SCREEN_WIDTH / 2 > mapwidth - 1) {
			cx = mapwidth - SCREEN_WIDTH;
		}
		else {
			cx = cx + 5 + SCREEN_WIDTH / 2 - SCREEN_WIDTH;
		}
	}
	else {
		cx = 0;
	}

	if (mapheight > SCREEN_HEIGHT)
	{
		if (cy < mapheight - SCREEN_HEIGHT / 2) {
			cy = cy - SCREEN_HEIGHT / 2;
		}
		else {
			cy = mapheight - SCREEN_HEIGHT;
		}
	}
	else {
		cy = mapheight > SCREEN_HEIGHT;
	}
	camera->SetCameraPosition((int)cx, (int)cy);

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Torch*>(objects[i]))
		{
			Torch* torch = dynamic_cast<Torch*>(objects[i]);
			if(torch->isColi==true)
			//Grid* grid = Grid::GetInstance();
				grid->deleteObject(objects[i]);
		}
		else if (dynamic_cast<Candle*>(objects[i]))
		{
			Candle* candle = dynamic_cast<Candle*>(objects[i]);
			if (candle->isColi == true)
				//Grid* grid = Grid::GetInstance();
				grid->deleteObject(objects[i]);
		}
	}
}

void CPlayScene::Render()
{
	map->DrawMap();
	player2->Render();
	//grid->GetListOfObjects(&objects);
	//for (int i = 0; i < objects.size() && dynamic_cast<Whip*>(objects[i]) == false; i++) {
	//	if (dynamic_cast<Torch*>(objects[i]))
	//		DebugOut(L"ve lua");
	//	objects[i]->Render();
	//	/*if (dynamic_cast<Torch*>(objects[i]))
	//		DebugOut(L"co ne \n");*/
	//}
	grid->GetListOfObjects(&objects);
	for (int i = 0; i < objects.size(); i++)
	{
		if(dynamic_cast<Whip*>(objects[i]) == false)
			objects[i]->Render();
	}

}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	//player = NULL;
	player2 = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::GetWhip()
{
	for (UINT i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT e = objects[i];

		if (dynamic_cast<Whip*>(e)) // if e là whip 
			GetPlayer2()->SetWhip((Whip*)e) ;
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon* simon = ((CPlayScene*)scence)->GetPlayer2();
	Game* game = Game::GetInstance();
	//((CPlayScene*)scence)->GetWhip();

	switch (KeyCode)
	{
	case DIK_X:
		simon->Jump();
		break;
	case DIK_A:
		simon->Reset();
		break;
	case DIK_Z:
		simon->Standing();
		break;
	case DIK_ESCAPE:
		DestroyWindow(game->getHwnd());
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	Game* game = Game::GetInstance();
	Simon* simon = ((CPlayScene*)scence)->GetPlayer2();
	// disable control key when Mario die 
	//if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetState(SIMON_STATE_WALK_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetState(SIMON_STATE_WALK_LEFT);
	}
	else
	{
		//if (simon->isOnStair)
			//simon->IdleOnStair();
		//else
		if(!simon->isOnStair)
			simon->Idle();
		
	}
	if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->canClimbDownStair)
		{
			simon->StairDown();
			
		}
		else 
		{
			if (simon->isOnStair)
				simon->StairDown();
			else
				simon->Idle();
		}
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (simon->canClimbUpStair)
		{
			simon->StairUp();
		}
		else
		{
			if (simon->isOnStair) {
				simon->StairUp();
			}
			else {
				simon->Idle();
			}
		}
	}
}
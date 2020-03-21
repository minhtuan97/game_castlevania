/* =============================================================
INTRODUCTION TO GAME PROGRAMMING SE102

SAMPLE 00 - INTRODUCTORY CODE

This sample illustrates how to:

1/ Create a window
2/ Initiate DirectX 9, Direct3D, DirectX Sprite
3/ Draw a static brick sprite to the screen

WARNING: This example contains a hell LOT of *sinful* programming practices
================================================================ */

#include <tchar.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Mario.h"
#include "Textures.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"00 - Intro"

#define BRICK_TEXTURE_PATH L"brick.png"
#define MARIO_TEXTURE_PATH L"mario.png"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 0, 0)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 90

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

Game* game;
Mario* mario;
//GameObject* brick;
//GameObject* mario;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else mario->SetState(MARIO_STATE_IDLE);
}

//nhận xử lý sự kiện cuae window, xử lý hằng đợi
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources. In this example, create a brick object and mario object
*/
void LoadResources()
{
	Textures* textures = Textures::GetInstance();
	
	textures->Add(ID_TEX_MARIO, L"textures\\mario.png", D3DCOLOR_XRGB(176, 224, 248));
	//textures->Add(ID_ENEMY_TEXTURE, L"textures\\enemies.png", D3DCOLOR_XRGB(156, 219, 239));
	//textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(156, 219, 239));

	Sprites* sprites = Sprites::GetInstance();
	Animations* animations = Animations::GetInstance();

	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);
	// readline => id, left, top, right 

	sprites->Add(10001, 246, 154, 259, 181, texMario);
	sprites->Add(10002, 275, 154, 290, 181, texMario);
	sprites->Add(10003, 304, 154, 321, 181, texMario);

	sprites->Add(10011, 186, 154, 199, 181, texMario);
	sprites->Add(10012, 155, 154, 170, 181, texMario);
	sprites->Add(10013, 125, 154, 140, 181, texMario);

	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	sprites->Add(20001, 300, 117, 315, 132, texMisc);
	sprites->Add(20002, 318, 117, 333, 132, texMisc);
	sprites->Add(20003, 336, 117, 351, 132, texMisc);
	sprites->Add(20004, 354, 117, 369, 132, texMisc);


	//mario = new Mario(MARIO_TEXTURE_PATH);
	//mario->SetPosition(10.0f, 130.0f);

	//brick = new GameObject(BRICK_TEXTURE_PATH);
	//brick->SetPosition(100.0f, 100.0f);
	LPANIMATION ani;

	/*ani = new Animation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new Animation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	
	ani = new Animation(100);
	ani->Add(20001,1000);
	ani->Add(20002);
	ani->Add(20003);
	ani->Add(20004);
	animations->Add(510, ani);

	mario = new Mario();
	mario->AddAnimation(500);
	mario->AddAnimation(501);
	mario->AddAnimation(510);*/

	ani = new Animation(100);
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new Animation(100);
	ani->Add(10011);
	animations->Add(401, ani);


	ani = new Animation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new Animation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	mario = new Mario();
	Mario::AddAnimation(400);		// idle right
	Mario::AddAnimation(401);		// idle left
	Mario::AddAnimation(500);		// walk right
	Mario::AddAnimation(501);		// walk left

	mario->SetPosition(0.0f, 100.0f);
	//mario->SetPosition(10.0f, 100.0f);
}

void Update(DWORD dt)
{
	mario->Update(dt);
	//brick->Update(dt);
}

/*
Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();
	
	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);


		mario->Render();
		//brick->Render();


		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			game->ProcessKeyboard();
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);
	//InitDirectX(hWnd);
	game = Game::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);

	LoadResources();
	Run();

	return 0;
}
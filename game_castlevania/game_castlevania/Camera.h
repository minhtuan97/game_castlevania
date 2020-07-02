#pragma once
#include <d3dx9.h>

class Camera
{
private:
	D3DXVECTOR3 cameraPosition;
	int width, height;
	static Camera* __instance;

public:
	Camera();
	static Camera* GetInstance();
	D3DXVECTOR3 GetPositionInCamera(D3DXVECTOR3 position);
	void SetCameraPosition(float x, float y);
	void Setwidthheight(int x, int y);
	D3DXVECTOR3 GetCameraPosition();
	~Camera();

};


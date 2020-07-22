#pragma once
#include "GameObject.h"

class Weapon:public GameObject
{
protected:
	bool isAttack;
	int aniFreeze;
	DWORD timeAttact=0;
public:
	bool isAllowToUse = false;
	bool isLopping = false;//trạng phái render khi đóng băng
	Weapon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void CollisionWithListObjects(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void setPosition(float _x, float _y, int _nx);
	int getIDFreeze() { return aniFreeze; }
	void setIDFreeze(int a) { aniFreeze = a; }

	bool GetAttack() { return isAttack; }
	void SetAttack(bool _isAttack) { isAttack = _isAttack; };
	virtual void SetFirst(bool _isFirst);
	virtual bool GetFirst();
	virtual void Reset(float x_t, float y_t, int nx_temp);
	DWORD GetLastTimeAttack() { return timeAttact; };
	~Weapon();
};


#include "BatBoss.h"
#include "define.h"
#include "Camera.h"

BatBoss::BatBoss()
{
	SetState(BOSSBAT_STATE_IDLE);
	isWaiting = true;
	yLine1 = 28;
	yLine2 = 72;
	xminLine2 = 577;
	xmaxLine2 = 720;
}

void BatBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	simon = Simon::GetInstance();

	float xmin = 690;
	float xmax = 768;
	float xsimon, ysimon;
	simon->GetPosition(xsimon, ysimon);
	if (isWaiting) {
		if (timeBeginState == 0) {
			if (xsimon >xmin) {
				timeBeginState = GetTickCount();
				Camera::GetInstance()->block = true;
			}
		}
		else if (GetTickCount() - timeBeginState > BOSSBAT_TIME_WAITING_ACTIVE) {
			isWaiting = false;
			//line = new CLine(x, y, randomX(), dyRepairToAttack);
			xLine2 = float_randomX(xminLine2, xmaxLine2);
			if (xLine2 - x < 0) vx = -vx;
			SetState(BOSSBAT_STATE_GOLINE2);
			vy = abs((yLine2-yLine1) * vx / (xLine2 - x));
		}
	}
	else {
		GameObject::Update(dt);

		if (GetTickCount() - timeBeginState > BOSSBAT_TIME_WAITING_ATTACK && state == BOSSBAT_STATE_IDLE)
		{
			SetState(BOSSBAT_STATE_DOWN_ATTACk);
			a = (y - ysimon) / (x * x - xsimon * xsimon);
			b = (y - a * x * x);
			if (xsimon - x < 0) vx = -vx;
			//vy = 10*vx*vx-30;
		}

		//if (GetTickCount() - timeBeginState < BOSSBAT_TIME_WAITING_AUTOGO && state == BOSSBAT_STATE_AUTOGO_REPAIR)return;
		//if (GetTickCount() - timeBeginState < BOSSBAT_TIME_WAITING_AUTOGO && state == BOSSBAT_STATE_AUTOGO_ATTACK)return;
		//if (GetTickCount() - timeBeginState < BOSSBAT_TIME_WAITING_FLY && state == BOSSBAT_STATE_FLY)return;
		//if (GetTickCount() - timeBeginState < BOSSBAT_TIME_WAITING_AUTOGO && state == BOSSBAT_STATE_IDLE) SetState(BOSSBAT_STATE_AUTOGO_ATTACK);

		if (isGoDown)
		{
			//dy = a * dx * dx + b;
			dy = a * x * x + b - y;
			//y = a * x * x + b;

		}
		else
			if (isGoUp)
			{
				dy =- dx*dx;
				//y = a * x * x + b;
			}
		//DebugOut(L"vx=%f, vy=%f, dx=%f, dy=%f\n", vx, vy, dx, dy);

		if (isAutoGo) {
			x += dx;
			y += dy;
			if(state== BOSSBAT_STATE_GOLINE2&&y > yLine2) 
			{
				SetState(BOSSBAT_STATE_IDLE);
				timeBeginState = GetTickCount();
			}
			if (state == BOSSBAT_STATE_DOWN_ATTACk && y > ysimon)
			{
				SetState(BOSSBAT_STATE_UP_ATTACk);
				isGoDown = false;
				isGoUp = true;
			}
			if (state == BOSSBAT_STATE_UP_ATTACk && y < yLine1)
			{
				SetState(BOSSBAT_STATE_IDLE);
				isWaiting = true;
			}
		}
		Camera* cam = Camera::GetInstance();
		float x_cam, y_cam;
		x_cam = cam->GetCameraPosition().x;
		y_cam = cam->GetCameraPosition().y;
		if (x < x_cam + 1)
			x = x_cam + 1;
		if (x > x_cam + SCREEN_WIDTH - 1 - BOSSBAT_BBOX_WIDTH)
			x = x_cam + SCREEN_WIDTH - 1 - BOSSBAT_BBOX_WIDTH;
	}
}

void BatBoss::Render()
{
	int ani;
	if (state == 0)
		ani = 0;
	else
		ani = 1;
    animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void BatBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y;
    right = x + BOSSBAT_BBOX_WIDTH;
    bottom = y + BOSSBAT_BBOX_HEIGHT;

}

int BatBoss::getAniId()
{
    return 0;
}

float BatBoss::randomX()
{
    return 0.0f;
}

float BatBoss::float_randomX(float min, float max)
{

	//srand(time(NULL));
	srand(time(0));
	int s = (int)(max - min + 1);
	int res =  rand() % s + min;
	DebugOut(L"res=%d\n", res);

	return res;
}

void BatBoss::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case BOSSBAT_STATE_IDLE:
		isAutoGo = false;
		vx = vy = 0;
		break;
	case BOSSBAT_STATE_GOLINE2:
		isAutoGo = true;
		isGoUp = false;
		isGoDown = false;
		vx = BOSSBAT_SPEED_AUTOGO_X;
		break;
	case BOSSBAT_STATE_DOWN_ATTACk:
		isAutoGo = true;
		isGoUp = false;
		isGoDown = true;
		vx = BOSSBAT_SPEED_AUTOGO_X;
		break;
	case BOSSBAT_STATE_UP_ATTACk:
		isAutoGo = true;
		isGoUp = true;
		isGoDown = false;
		break;
	case BOSSBAT_STATE_UP_LINE1:
		isAutoGo = true;
		isGoUp = false;
		isGoDown = false;
		vx = BOSSBAT_SPEED_X;
		break;
	}
}

void BatBoss::beAttack()
{
}

BatBoss::~BatBoss()
{
}

#include "HolyWater.h"
#include "define.h"
#include "Brick.h"

HolyWater::HolyWater()
{
	Weapon::Weapon();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id_HolyWater);
	SetAnimationSet(ani_set);
	//timeStartFire = GetTickCount();
	denta_x = 16;
	state = 0;
}
void HolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isAttack) return;
	if (state == 0)
	vx = HolyWater_SPEED_X * nx;
	//Simple fall down
	
		vy = Fire_GRAVITY;
	GameObject::Update(dt); // cập nhật thời gian, vận tốc



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//if (isAttack)
		CalcPotentialCollisions(coObjects, coEvents);


	if (coEvents.size() == 0)
	{
		x += dx;
		denta_x = denta_x - abs(dx);
		if (denta_x <= 0)
			y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		
		// Collision logic with other objects
		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj)) // if e->obj is Brick
			{
				if (state == 0)
					timeStartFire = GetTickCount();
				state = 1;
				vy = 0;
				vx = 0;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	Camera* cam = Camera::GetInstance();
	float xcam = cam->GetCameraPosition().x;
	float ycam = cam->GetCameraPosition().y;

	/*if (x < xcam || x + HolyWater_BBOX_WIDTH > xcam + SCREEN_WIDTH) {
		if (!isFirst) {
			isFirst = true;
			nx = -nx;
			if (x < xcam)
				x = 1;
			else
				x = xcam + SCREEN_WIDTH - HolyWater_BBOX_WIDTH - 10;
		}
		else
			SetAttack(false);
	}*/
	//else if (isAttack) {
	//	vx = HolyWater_SPEED * nx;
	//	GameObject::Update(dt); // cập nhật thời gian, vận tốc
	//	x += dx;
	//}
	if (isAttack && timeStartFire > 0 && (GetTickCount() - timeStartFire > WATERFIRE_ATTACK_TIME))
		isAttack = false;
}

void HolyWater::Render()
{
	if (!isAttack) return;

	animation_set->at(state)->Render(x, y);
	RenderBoundingBox();
}

void HolyWater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 13;
}

void HolyWater::setPosixtion(float _x, float _y, int _nx)
{
}

void HolyWater::SetFirst(bool _isFirst)
{
	isFirst = _isFirst;
}

bool HolyWater::GetFirst()
{
	return isFirst;
}

void HolyWater::Reset(float x_t, float y_t, int nx_temp)
{
	SetAttack(true);
	SetPosition(x_t, y_t);
	this->nx = nx_temp;
	SetFirst(false);
	denta_x = 16;
	state = 0;
	timeStartFire = 0;

}

HolyWater::~HolyWater()
{
}
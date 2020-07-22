#include "Boomerang.h"
#include "define.h"
#include "Brick.h"

Boomerang::Boomerang()
{
	Weapon::Weapon();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id_Boomberang);
	SetAnimationSet(ani_set);
}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isAttack) return;
	vx = BOOMERANG_SPEED * nx;
	GameObject::Update(dt); // cập nhật thời gian, vận tốc

	// Simple fall down
	//vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (isAttack)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	//if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj)) // if e->obj is Brick
			{
				if (!isFirst) {
					isFirst = true;
					this->nx = -this->nx;
				}
				else
					SetAttack(false);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	Camera* cam = Camera::GetInstance();
	float xcam = cam->GetCameraPosition().x;
	float ycam = cam->GetCameraPosition().y;

	if (x < xcam || x + BOOMERANG_BBOX_WIDTH > xcam + SCREEN_WIDTH) {
		if (!isFirst) {
			isFirst = true;
			nx = -nx;
			if (x < xcam)
				x = 1;
			else
				x = xcam + SCREEN_WIDTH - BOOMERANG_BBOX_WIDTH - 10;
		}
		else
			SetAttack(false);
	}
	//else if (isAttack) {
	//	vx = BOOMERANG_SPEED * nx;
	//	GameObject::Update(dt); // cập nhật thời gian, vận tốc
	//	x += dx;
	//}
}

void Boomerang::Render()
{
	if (!isAttack) return;

	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
}

void Boomerang::setPosixtion(float _x, float _y, int _nx)
{
}

void Boomerang::SetFirst(bool _isFirst)
{
	isFirst = _isFirst;
}

bool Boomerang::GetFirst()
{
	return isFirst;
}

void Boomerang::Reset(float x_t, float y_t, int nx_temp)
{
	SetAttack(true);
	SetPosition(x_t, y_t);
	this->nx = nx_temp;
	SetFirst(false);

}

Boomerang::~Boomerang()
{
}

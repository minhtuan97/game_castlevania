#include "Axe.h"
#include "define.h"
#include "Brick.h"

Axe::Axe()
{
	Weapon::Weapon();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id_Axe);
	SetAnimationSet(ani_set);
	//timeStartFire = GetTickCount();
	denta_x =48;
}
void Axe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isAttack) return;
	vx = Axe_SPEED_X * nx;
	//Simple fall down

	vy = Axe_SPEED_Y * ny;
	GameObject::Update(dt); // cập nhật thời gian, vận tốc

	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();

	// turn off collision when die 
	//if (isAttack)
	//CalcPotentialCollisions(coObjects, coEvents);


	//if (coEvents.size() == 0)
	//{
		x += dx;
		y += dy;
		denta_x = denta_x - abs(dx);
		if (denta_x <= 0)
			ny = 1;
		else
			ny = -1;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;

	//	// TODO: This is a very ugly designed function!!!!
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	//	// block every object first!
	//	x += min_tx * dx + nx * 0.4f;
	//	y += min_ty * dy + ny * 0.4f;

	//	if (nx != 0) vx = 0;
	//	if (ny != 0) vy = 0;



	//	// Collision logic with other objects

	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

	//		if (dynamic_cast<Brick*>(e->obj)) // if e->obj is Brick
	//		{
	//			isAttack = false;
	//			//vy = 0;
	//			//vx = 0;
	//		}
	//	}
	//}

	//// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	Camera* cam = Camera::GetInstance();
	float xcam = cam->GetCameraPosition().x;
	float ycam = cam->GetCameraPosition().y;

	if (x < xcam || x + Axe_BBOX_WIDTH > xcam + SCREEN_WIDTH||y>ycam+SCREEN_HEIGHT) {		
			SetAttack(false);
	}
	//else if (isAttack) {
	//	vx = Axe_SPEED * nx;
	//	GameObject::Update(dt); // cập nhật thời gian, vận tốc
	//	x += dx;
	//}
	//if (isAttack && timeStartFire > 0 && (GetTickCount() - timeStartFire > WATERFIRE_ATTACK_TIME))
		//isAttack = false;
}

void Axe::Render()
{
	if (!isAttack) return;

	animation_set->at(0)->Render(x,y);
	//RenderBoundingBox();
}

void Axe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + Axe_BBOX_WIDTH;
	bottom = y + Axe_BBOX_HEIGHT;
}

void Axe::setPosixtion(float _x, float _y, int _nx)
{
}

void Axe::SetFirst(bool _isFirst)
{
	isFirst = _isFirst;
}

bool Axe::GetFirst()
{
	return isFirst;
}

void Axe::Reset(float x_t, float y_t, int nx_temp)
{
	SetAttack(true);
	SetPosition(x_t, y_t);
	this->nx = nx_temp;
	SetFirst(false);
	denta_x = 48;

}

Axe::~Axe()
{
}
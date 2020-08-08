#include "Board.h"

Board::Board()
{
	SetPosition(0, 0);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ANIMATIONSET_BOARD);
	SetAnimationSet(ani_set);
	heartCollect = new CNumber(0);
}

void Board::Render(Simon* simon, int state, int RemainingTime, GameObject* boss)
{
	Camera* cam = Camera::GetInstance();
	int ani = 0;
	animation_set->at(ani)->Render(x+cam->GetCameraPosition().x, y);

	//DebugOut(L"tim an duoc: %d\n", simon->HeartCollect);
	heartCollect->num = simon->HeartCollect;
	heartCollect->Render();

}

void Board::Render()
{
}

void Board::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}


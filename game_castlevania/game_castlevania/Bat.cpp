#include "Bat.h"
#include "Simon.h"
#include "Grid.h"
#include "Map.h"

Bat::Bat(float x, float y)
{
	start_x = x;
	start_y = y;
	vx = vy = 0;
	this->x = x;
	this->y = y;
}

void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	float simonX=0, simonY=0;

	Simon::GetInstance()->GetPosition(simonX, simonY);
	//DebugOut(L"simonX:%f, simonY:%f, x:%f, y:%f, left_max=%f, right_max=%f, top_max=%f, bot_max=%f\n", simonX, simonY, x, y, left_max, right_max, top_max, bot_max);

	/*if (y - simonY >= -20)
	{
		vy = -abs(vy) - dt;
		vy = -0.02f;
	}
	else if (y - simonY <= 20)
	{
		vy = abs(vy)+dt;
		vy = 0.02f;
	}
	else
		vy = 0;*/
	if (left_max < simonX && simonX < right_max && top_max < simonY && simonY < bot_max)
	{
		vx = 0.03f;
		vy = 0.02;
		if (y_de == NULL)
			y_de = simonY;
	}
	if (y<simonY + 10 && y>simonY - 10)
	{
		if (vx > 0)
			y = simonY;
		vy = 0;
	}
	
	GameObject::Update(dt); // Update dt, dx, dy
	//DebugOut(L"vx=%f,vy=%f,dx:%f, dy:%f\n", vx,vy,dx,dy);
	y += dy;
	x += dx;
	if (y > y_de + 42) y = y_de + 42;
	if (y < 1) y = 1;
	Grid* grid = Grid::GetInstance();
	Map* map = Map::GetInstance();
	if (x > map->GetWidth() - 10)
		grid->deleteObject(this);
	else
		grid->Update(this);

}

void Bat::Render()
{
	animation_set->at(0)->Render(x,y);
	//RenderBoundingBox();
}

void Bat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 16;
	bottom = top + 16;
}

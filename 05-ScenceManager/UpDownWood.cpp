#include "UpDownWood.h"
#include"Game.h"

void CUpDownWood::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	top = y;
	left = x;
	right = left + this->Width;
	bottom = top + this->Height;
}

void CUpDownWood::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health == 0)
		IsDie = true;
	if (IsDie)
		return;
	CGameObject::Update(dt, coObjects);
	y += dy;
	x += dx;
	int x1 = CGame::GetInstance()->GetCamX();
	if (this->x + Width < x1)
	{
		this->health == 0;
		//IsDie = true;
	}
	
}

void CUpDownWood::Render()
{
	if (IsDie)
		return;
	int ani = WOOD_ANI;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CUpDownWood::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WOOD_STATE_DOWN:
	{
		this->state = WOOD_STATE_DOWN;
		vy = WOOD_VX;
		vx = 0;
		break;
	}
	case WOOD_STATE_NORMAL:
	{
		this->state = WOOD_STATE_NORMAL;
		break;
	}
	}
}

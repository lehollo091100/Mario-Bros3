#include "PBrick.h"

void CPBrick::SetState(int state)
{
	switch (state)
	{
	case CPBRICK_STATE_NORMAL:
		vy = 0;
		this->state = CPBRICK_STATE_NORMAL;
		break;
	case CPBRICK_STATE_NOTHINGLEFT:
		this->state = CPBRICK_STATE_NOTHINGLEFT;
		vy = 0;
		//SubHealth(1);
		break;
	case CPBRICK_STATE_COLLISION:
	{
		this->state = CPBRICK_STATE_COLLISION;
		break;
	}
	}

}

void CPBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
}

void CPBrick::Render()
{
	int ani = 0;
	if (state == CPBRICK_STATE_NOTHINGLEFT||state==CPBRICK_STATE_COLLISION)
	{
		ani = 1;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CPBrick::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + CPBRICK_BBOX_WIDTH;
	b = y + CPBRICK_BBOX_WIDTH;
}

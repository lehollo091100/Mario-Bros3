#include "ShinningBrick.h"
CShinningBrick::CShinningBrick()
{
	type = GType::SHINNINGBRICK;
	width = QBRICK_BBOX_WIDTH;
	height = QBRICK_BBOX_HEIGHT;
	SetHealth(2);
	//state = 1;
	//vy = 0;
	SetState(BRICK_STATE_NORMAL);
}
void CShinningBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (health == 0)
	{
		IsDie=true;
	}
	if (IsDie)
	{
		//DebugOut(L"LINE 20 SBIRCK\n");
		return;
	}
	vy = 0;
	CGameObject::Update(dt);
	y += dy;

}
void CShinningBrick::Render()
{
	if (IsDie)
	{
		//DebugOut(L"LINE 20 SBIRCK\n");
		return;
	}
	int ani = 0;
	
	/*if (state == BRICK_STATE_NOTHINGLEFT)
	{
		ani = 1;
	}*/
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CShinningBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
void CShinningBrick::SetState(int state)
{
	switch (state)
	{
	case BRICK_STATE_NORMAL:

		this->state = BRICK_STATE_NORMAL;
		break;
	case SBRICK_STATE_NOTHINGLEFT:
		this->state = SBRICK_STATE_NOTHINGLEFT;
		//this->SubHealth(1);
		//SubHealth(1);
		break;
	case SBRICK_STATE_DISAPPEAR:
		this->state = SBRICK_STATE_DISAPPEAR;
		break;
	}
}


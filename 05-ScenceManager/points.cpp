#include "points.h"

void Cpoints::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = top;
}

void Cpoints::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health == 0)
		IsDie = true;
	if (IsDie)
		return;
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (startY==0)
	{
		startY = y;
	}
	if (startY - y > MAXHEIGHT)
	{
		this->SubHealth(1);
	}
	//DebugOut(L"points:%d\n", this->health);

}

void Cpoints::Render()
{
	if (health == 0)
		return;
	int ani = POINT_ANI_APPEAR;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void Cpoints::SetState(int state)
{

	CGameObject::SetState(state);
	switch (state)
	{
	case POINT_STATE_UP:
		this->state = POINT_STATE_UP;
		break;
	}
}


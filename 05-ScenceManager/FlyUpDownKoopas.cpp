#include "FlyUpDownKoopas.h"
#include"Utils.h"

void FlyUpDownKoopas::SetState(int state)
{
	
	CGameObject::SetState(state);
	switch (state)
	{
	case FLY_STATE:
	{
		this->state = FLY_STATE;
		break;
	}
	case DISAPPEAR_STATE:
	{
		this->state = DISAPPEAR_STATE;
		break;
	}
	default:
		break;
	}
}

void FlyUpDownKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health == 0)
	{
		IsDie = true;
	}
	if (IsDie)
	{
		return;
	}
	//vy = ny * 0.05f;
	if (startY==0)
	{
		startY = this->y;
	}
	CGameObject::Update(dt, coObjects);
	y += dy;
	x += dx;
	if (y >= 380||y<=300)
	{
		this->ny = -ny;
		vy = ny * KOOPAS_VY;
	}
}

void FlyUpDownKoopas::Render()
{
	if (IsDie)
	{
		return;
	}
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void FlyUpDownKoopas::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + BBOX_WIDTH;
	b = t + BBOX_HEIGHT;
}

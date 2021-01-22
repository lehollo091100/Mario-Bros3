#include "Leaf.h"

void CLeaf::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!IsDie) {
		left = x;
		top = y;
		right = left + LEAF_BBOX_WIDTH;
		bottom = top + LEAF_BBOX_WIDTH;
	}
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"leafhealth:%d\n", this->health);
	if (health==0)
	{
		IsDie = true;
	}
	if (IsDie)
	{
		return;
	}
	CGameObject::Update( dt, coObjects);
	x += dx; y += dy;
	if (state != LEAF_STATE_UP)
		vx = nx* SPEEDX;
	if (startX == 0)
		startX = x;
	if (startY == 0)
		startY = y;
	//DebugOut(L"startY of leaf:%f vy:%f:", startY, vy);
	if (y <= startY - MAXHEIGHT)
		SetState(LEAF_STATE_FALLING);
	if (nx>0)
	{
		if (x >= startX + RANGE)
		{
			nx = -nx;
			vx = nx * SPEEDX;
		}
	}
	if (nx < 0)
	{
		if (this->x <= startX)
		{
			nx = -nx;
			vx = nx * SPEEDX;
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	coEventsResult.clear();
	CalcPotentialCollisions(coObjects, coEvents);
}

void CLeaf::Render()
{
	if (IsDie)
	{
		return;
	}
	int ani = 0;
	if (nx > 0)
		ani = LEAF_ANI_RIGHT;
	if (nx<0)
	{
		ani = LEAF_ANI_RIGHT;
	}
	animation_set->at(ani)->Render(x, y);
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_UP:
		//diestart = GetTickCount64();
		//DebugOut(L"state goomba die\n");
		this->state = LEAF_STATE_UP;
		IsUp = true;
		vy = -SPEEDUP;
		break;
	case LEAF_STATE_FALLING:
		ny = 1;
		vy = ny*SPEEDDOWN;
		this->state = LEAF_STATE_FALLING;
		IsFalling = true;
		IsUp = false;
		break;
		//vx = -GOOMBA_WALKING_SPEED;
	case LEAF_STATE_UP_FALLING:
		vy = -0.01f;
		this->state = LEAF_STATE_UP_FALLING;
		IsFalling = false;
		IsFallUp = true;
		break;
	}
}

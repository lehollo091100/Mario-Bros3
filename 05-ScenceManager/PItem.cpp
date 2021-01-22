#include "PItem.h"

void CPItem::SetState(int state)
{
	switch (state)
	{
	case PITEM_STATE_NORMAL:
		vy = 0;
		this->state = PITEM_STATE_NORMAL;
		break;
	case PITEM_STATE_COLLISION:
		this->state = PITEM_STATE_COLLISION;
		vy = 0;
		//SubHealth(1);
		break;
	}
}

void CPItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;

}

void CPItem::Render()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(57);
	SetAnimationSet(ani_set);
	int ani = 0;
	if (state == PITEM_STATE_COLLISION)
	{
		ani = 1;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CPItem::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (state==PITEM_STATE_COLLISION)
	{
		return;
	}
	l = x;
	t = y;
	if (state == PITEM_STATE_NORMAL) {
		r = l + PITEM_BBOX_WIDTH;
		b = t + PITEM_BBOX_WIDTH;
	}
	
}

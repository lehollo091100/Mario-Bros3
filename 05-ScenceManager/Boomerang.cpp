#include "Boomerang.h"
#include"Utils.h"

void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_STATE_ATTACK:
		vx = 0.12f;
		vy = -0.05f;
		break;
	case BOOMERANG_STATE_DOWN_ATTACK:
		vx = 0.08f;
		vy = 0.1f;
		break;
	case BOOMERANG_STATE_MOVING_BACK:
		vy = 0;
		vx = -0.1;
		break;
	}
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health == 0)
	{
		return;
	}
	if (maxX == 0)
	{
		maxX = x + 100;
	}
	if (maxY == 0)
	{
		maxY = y + 10;
	}
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (state != BOOMERANG_STATE_MOVING_BACK)
	{
		vy += GRAVITY * dt;
	}
	if (state == BOOMERANG_STATE_ATTACK)
	{
		if (x > maxX)
		{
			SetState(BOOMERANG_STATE_DOWN_ATTACK);
		}
	}
	if (state==BOOMERANG_STATE_DOWN_ATTACK)
	{
		if (y >= maxY)
		{
			SetState(BOOMERANG_STATE_MOVING_BACK);
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	coEventsResult.clear();




	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->nx != 0) {
				if (e->obj->GetType() == GType::TURTLE)
				{
					SetHealth(0);
				}
				/*else {
					x += dx;
					y += dy;
				}*/
			}

		}
	}
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) != NULL && coObjects->at(i)->GetHealth() > 0)
			if ((isCollisionWithObj(coObjects->at(i))->t > 0) && isCollisionWithObj(coObjects->at(i))->t < 1)
			{
				switch (coObjects->at(i)->GetType())
				{
				case GType::TURTLE:
				{
					if (state==BOOMERANG_STATE_MOVING_BACK)
					{

						SetHealth(0);
					}
					break;
				}
				}
			}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBoomerang::Render()
{
	if (health==0)
	{
		return;
	}
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(21);
	SetAnimationSet(ani_set);
	animation_set->at(1)->Render(x, y);
	RenderBoundingBox();
}

void CBoomerang::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = l + BOMERANG_BBOX_WIDTH;
	b = t + BOMERANG_BBOX_HEIGHT;
}

#include "Goomba.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
	SetHealth(2);
	type = GType::GOOMBA;
	nx = -1;
	vx = nx * GOOMBA_WALKING_SPEED;
	Isdie = false;
	//IsWalking = true;
	time = 0;
}


void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (health == 0)
		IsDie = true;
	if (IsDie)
		return;
	CGameObject::Update(dt, coObjects);
	vy += 0.015f * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	DWORD now = GetTickCount64();
	//time = now - diestart;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	//x += dx;
	//y += dy;

	//DebugOut(L"time:%d\n",time);
	if (IsWalking) {
		state = GOOMBA_STATE_WALKING;
		if (nx == -1)
			if (x <= 0)
			{
				nx = -nx;
				vx = nx * GOOMBA_WALKING_SPEED;
			}
		/*if (nx == 1)
			if (x >= 150)
			{
				nx = -nx;
				vx = nx * GOOMBA_WALKING_SPEED;
			}*/

	}

	if (health == 1)
	{
		if (IsWalking)
		{
			vx = 0;
			//DebugOut(L"line 50\n");
			IsWalking = false;
			SetState(GOOMBA_STATE_DIE);
		}
	}
	if (Isdie)
	{
		vx = 0;
		this->state = GOOMBA_STATE_DIE;
		if (time <= 30)
		{
			time++;
		}
		else {
			SubHealth(1);
		}
	}



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
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->nx != 0) {
				if (e->obj->GetType() == GType::BRICK||e->obj->GetType() == GType::PIPE||e->obj->GetType()==GType::QUESTIONBRICK)
				{
					this->nx = -this->nx;
					this->vx = this->nx*GOOMBA_WALKING_SPEED;
				}
				if (e->obj->GetType() == GType::COLORBRICK)
				{
					x += dx;
				}
				/*if (e->obj->GetType()==GType::FLYKOOPAS)
				{
					vy = 0;
				}*/
				/*else
				{
					x += dx;

				}*/
				
			}
			if (e->ny != 0)
			{
				if (e->obj->GetType() == GType::ITEM||e->obj->GetType()==GType::FLYKOOPAS)
				{
					vy= 0;
				}
			}

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	if (IsDie)
	{
		return;
	}
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		//diestart = GetTickCount64();
		//DebugOut(L"state goomba die\n");
		Isdie = true;
		
		break;
	case GOOMBA_STATE_WALKING:
		IsWalking = true;
		break;
		//vx = -GOOMBA_WALKING_SPEED;
	}
}
void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = top + GOOMBA_BBOX_HEIGHT;
}

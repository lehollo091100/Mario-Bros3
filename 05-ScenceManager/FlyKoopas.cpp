#include "FlyKoopas.h"

CFlyKoopas::CFlyKoopas(CMario * m, int range)
{
	//health = 1;
	type = GType::FLYKOOPAS;
	SetState(FLYKOOPAS_STATE_FLY);
	nx = -1;
	vx = nx * FLYKOOPAS_WALKING_SPEED;
	SetHealth(2);
}
void CFlyKoopas::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + FLYKOOPAS_BBOX_WIDTH;
	bottom = y + FLYKOOPAS_BBOX_HEIGHT;
}

void CFlyKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health <= 0)
		IsDie=true;
	if (IsDie)
		return;
	if (state == FLYKOOPAS_STATE_DISAPPEAR)
		health-=1;
	CGameObject::Update(dt, coObjects);
	vy += FLYKOOPAS_GRAVITY * dt;
	

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
			if (e->ny != 0) {
				if (e->obj->GetType() == GType::COLORBRICK || e->obj->GetType() == GType::BRICK || e->obj->GetType() == GType::QUESTIONBRICK)
				{
					this->vy = -FLYKOOPAS_DEFLECT_SPEED;
					//DebugOut(L"LINE 60\n");
				}

			}
			if (e->obj->GetType() == GType::GOOMBA)
			{
				x += dx;
				y += dy;
			}
				
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CFlyKoopas::Render()
{
	if (IsDie)
		return;
	if (state == FLYKOOPAS_STATE_DISAPPEAR)
		return;
	int ani = FLYKOOPAS_ANI_FLY;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}


void CFlyKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case FLYKOOPAS_STATE_FLY:
		{
			this->state = FLYKOOPAS_STATE_FLY;
			//vx = nx * FLYKOOPAS_WALKING_SPEED;
			break;
		}
		case FLYKOOPAS_STATE_DISAPPEAR:
		{
			this->state = FLYKOOPAS_STATE_DISAPPEAR;
			break; 
		}
	}
}

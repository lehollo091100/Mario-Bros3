#include "Koopas.h"
#include "Utils.h"
CKoopas::CKoopas(int range)
{
	this->range = range;
	SetState(KOOPAS_STATE_WALKING);
	StartPX = 0;
	nx = 1;
	type = GType::KOOPAS;
	vx = KOOPAS_WALKING_SPEED;
	SetHealth(2);
	level = KOOPAS_LEVEL_NORMAL;
	IsWalking = true;
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//if (health <= 0)
	//	return;
	//float cam_x = CGame::GetInstance()->GetCamX();
	//float cam_w = CGame::GetInstance()->GetScreenWidth();
	////out cam
	//if (x > cam_x + cam_w || x < cam_x)
	//	health = 0;
	if (StartPX == 0)
		StartPX = x;
	if (health == 2) {
		if (nx == -1)
			if ((x - StartPX) <= -range) {
				nx = -nx;
				vx = nx * KOOPAS_WALKING_SPEED;
			}
		if (nx == 1)
			if ((x + 14 - StartPX) >= range) {
				nx = -nx;
				vx = nx * KOOPAS_WALKING_SPEED;
			}
	}
	if (health == 1)
	{
		//SetLevel(KOOPAS_LEVEL_DEFEND);
		if (state == KOOPAS_STATE_WALKING)
		{
			/*y += 3;*/
			state = KOOPAS_STATE_DIE;
			//vy = 0;
		}
	}
	if (health==0)
	{
		if (state == KOOPAS_STATE_DIE)
		{
			state = KOOPAS_STATE_ATTACK;
			//vx = nx * KOOPAS_WALKING_SPEED * 2;
		}
	}
	//if(nx == 1 && abs(x - 14 - StartPX) >= range) {
	//	nx = -nx;
	//	vx = nx * KOOPAS_WALKING_SPEED;
	//}
	CGameObject::Update(dt, coObjects);
	vy += 0.015f * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

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
			if (e->obj->GetType() == GType::BRICK) {
				if (e->nx != 0 &&(state==KOOPAS_STATE_WALKING||state==KOOPAS_STATE_ATTACK))
				{
					nx = -nx;
					vx = nx * KOOPAS_WALKING_SPEED;
				}
			}

			
			if (e->obj->GetType() == GType::COLORBRICK)
			{
				if (nx != 0)
				{
					x += dx;
				}
				/*if (e->ny == -1)
				{
					state = KOOPAS_STATE_WALKING;
					vx += dx;
					vy = 0;
				}*/
			}
			else {
				x += dx;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_ATTACK) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:

		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		//this->state = KOOPAS_STATE_WALKING;
		IsWalking = true;
		break;
	case KOOPAS_STATE_ATTACK:
		IsAttacking;
		break;
	}

}
void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE||state==KOOPAS_STATE_ATTACK)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}
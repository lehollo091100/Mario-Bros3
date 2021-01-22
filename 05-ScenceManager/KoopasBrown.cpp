#include "KoopasBrown.h"

CKoopasBrown::CKoopasBrown(CMario * m, int range)
{
	type = GType::KOOPASBROWN;
	SetState(KOOPASBROWN_STATE_WALKING);
	StartPX = 0;
	nx = 1;
	vx = KOOPASBROWN_WALKING_SPEED;
	SetHealth(3);
	IsWalking = true;
	IsDie = IsDefending = false;
	mario = m;
	level = KOOPASBROWN_LEVEL_NORMAL;
}
void CKoopasBrown::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + KOOPASBROWN_BBOX_WIDTH;

	if (state == KOOPASBROWN_STATE_DEFEND || state == KOOPASBROWN_STATE_ATTACK)
		bottom = top + KOOPASBROWN_BBOX_HEIGHT_DIE;
	else
		bottom = top + KOOPASBROWN_BBOX_HEIGHT;
}

void CKoopasBrown::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (health <= 0)
	{
		IsDie = true;
	}
	if (IsDie || IsHeld)
		return;

	float cam_x = CGame::GetInstance()->GetCamX();
	float cam_w = CGame::GetInstance()->GetScreenWidth();
	//out cam
	if (IsAttacking || IsUpAttacking) {
		if (x > cam_x + cam_w || x < cam_x || x < 0)
			health = 0;
		//DebugOut(L"OUT CAM LINE 31 KOOPAS\n");
	}
	if (StartPX == 0)
		StartPX = x;
	//if (health == 3) {

	if (IsWalking) {
		//DebugOut(L"LINE 45\n");
		if (nx == -1)
			if ((x - StartPX) <= -range) {
				nx = -nx;
				vx = nx * KOOPASBROWN_WALKING_SPEED;
			}
		if (nx == 1)
			if ((x + 14 - StartPX) >= range) {
				nx = -nx;
				vx = nx * KOOPASBROWN_WALKING_SPEED;
			}
	}
	//}
	//if (health == 2)
	//{
	//	//SetLevel(KOOPASBROWN_LEVEL_DEFEND);
	//	//DebugOut(L"line 68\n");
	//	//vx = 0;
	//	//SetState(KOOPASBROWN_STATE_DEFEND);
	//}
	//if (health == 1)
	//{
	//	//IsDefending = false;
	//	if (state != KOOPASBROWN_STATE_ATTACK)
	//		SetState(KOOPASBROWN_STATE_ATTACK);
	//	//DebugOut(L"health==1 line 56 \n");
	//}

	//DebugOut(L"%d\n", state);
	if (IsDefending || IsUp)
	{
		DebugOut(L"line 76 \n");
		//state = KOOPASBROWN_STATE_DEFEND;
		vx = 0;
		if (time == 0)
		{
			time = GetTickCount64();
		}
		if (GetTickCount64() - time > 3000)
		{
			time = 0;
			SetState(KOOPASBROWN_STATE_WALKING);
			this->y -= 10;
		}
	}

	if (IsAttacking || IsUpAttacking)
	{
		//state = KOOPASBROWN_STATE_ATTACK;
		vx = nx * KOOPASBROWN_ATTACK_SPEED;
		//DebugOut(L"LINE 72 ATTACK STATE\n");
	}
	//if(nx == 1 && abs(x - 14 - StartPX) >= range) {
	//	nx = -nx;
	//	vx = nx * KOOPASBROWN_WALKING_SPEED;
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
			if (state == KOOPASBROWN_STATE_WALKING)
			{

				if (e->obj->GetType() == GType::BRICK||e->obj->GetType()==GType::COLORBRICK) {
					if (e->nx != 0)
					{
						this->nx = -this->nx;
						vx = this->nx * KOOPASBROWN_WALKING_SPEED;
					}
				}
				else {
					x += dx;
				}
			}
			if (state == KOOPASBROWN_STATE_ATTACK || state == KOOPASBROWN_STATE_UP_ATTACK)
			{
				if (e->nx != 0) {
					if (e->obj->GetType() == GType::BRICK || e->obj->GetType() == GType::QUESTIONBRICK || e->obj->GetType() == GType::PIPE) {
						x -= -(this->nx) * 0.04f;
						this->nx = -this->nx;
						this->vx = this->nx * KOOPASBROWN_ATTACK_SPEED;
						DebugOut(L"line 153\n");


					}
					if (e->obj->GetType() == GType::GOOMBA)
					{

						e->obj->SubHealth(1);

					}
					else { x += dx; }
				}
			}

		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopasBrown::Render()
{
	if (health == 0)
		return;
	int ani = 0;
	if (state == KOOPASBROWN_STATE_DEFEND) {
		ani = KOOPASBROWN_ANI_DEFEND;
	}
	if (state == KOOPASBROWN_STATE_UP)
	{
		ani = KOOPASBROWN_ANI_DIE;
	}
	if (vx > 0 && state == KOOPASBROWN_STATE_WALKING)
	{
		ani = KOOPASBROWN_ANI_WALKING_RIGHT;
		DebugOut(L"LINE 193\n");
	}
	if (vx < 0 && state == KOOPASBROWN_STATE_WALKING) {
		ani = KOOPASBROWN_ANI_WALKING_LEFT;
		DebugOut(L"LINE 197\n");
		//ani = KOOPASBROWN_ANI_WALKING_RIGHT;
	}
	if (state == KOOPASBROWN_STATE_ATTACK)
	{
		ani = KOOPASBROWN_ANI_ATTACK;
	}
	if (state == KOOPASBROWN_STATE_UP_ATTACK)
	{
		ani = KOOPASBROWN_ANI_DIE_ATTACK;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}


void CKoopasBrown::SetState(int state)
{

	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPASBROWN_STATE_DEFEND:
		//DebugOut(L"LINE 147 KOOPAS.CPP KOOPAS STATE DIE \n");
		IsDefending = true;
		IsWalking = false;
		IsAttacking = false;
		IsUpAttacking = false;
		IsUp = false;
		this->state = KOOPASBROWN_STATE_DEFEND;
		break;
	case KOOPASBROWN_STATE_WALKING:
		this->state = KOOPASBROWN_STATE_WALKING;
		IsWalking = true;
		IsDefending = false;
		IsAttacking = false;
		IsUpAttacking = false;
		IsUp = false;
		vx = nx * KOOPASBROWN_WALKING_SPEED;
		break;
	case KOOPASBROWN_STATE_ATTACK:
		IsDefending = false;
		IsWalking = false;
		IsAttacking = true;
		//IsUpAttacking = false;
		//IsUp = false;
		//DebugOut(L"CASE ATTACK \n");
		break;
	case KOOPASBROWN_STATE_UP:
		this->y -= 15;
		//IsDefending = false;
		IsWalking = false;
		//IsAttacking = false;
		IsUpAttacking = false;
		IsUp = true;
		vx = 0;
		//this->state = KOOPASBROWN_STATE_UP;
		break;
	case KOOPASBROWN_STATE_UP_ATTACK:
		//this->state = KOOPASBROWN_STATE_UP_ATTACK;
		//IsDefending = false;
		IsWalking = false;
		//IsAttacking = false;
		IsUpAttacking = true;
		IsUp = false;

		break;
	}
}

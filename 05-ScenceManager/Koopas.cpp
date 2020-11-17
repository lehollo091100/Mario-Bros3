#include "Koopas.h"
#include "Utils.h"
CKoopas::CKoopas(CMario *m, int range )
{
	this->range = range;
	SetState(KOOPAS_STATE_WALKING);
	StartPX = 0;
	nx = 1;
	type = GType::KOOPAS;
	vx = KOOPAS_WALKING_SPEED;
	SetHealth(3);
	IsDefending = false;
	level = KOOPAS_LEVEL_NORMAL;
	IsWalking = true;
	mario = m;
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	//DebugOut(L"2:%f\n", vx);
	if (health <= 0)
	{
		IsDie = true;
	}
	if (IsDie||IsHeld)
		return;
	
	float cam_x = CGame::GetInstance()->GetCamX();
	float cam_w = CGame::GetInstance()->GetScreenWidth();
	//out cam
	if(IsAttacking){
	if (x > cam_x + cam_w || x < cam_x||x<0)
		health = 0;
		//DebugOut(L"OUT CAM LINE 31 KOOPAS\n");
	}
	if (health == 3)
	{
		//DebugOut(L"HEAL =3\n");
	}
	if (health == 2)
	{
		//DebugOut(L"HEAL =2\n");
	}
	if (StartPX == 0)
		StartPX = x;
	if (health == 3) {

		if (IsWalking) {
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
	}
	if (health == 2)
	{
		//SetLevel(KOOPAS_LEVEL_DEFEND);
		IsWalking == false;
		IsAttacking = false;
		//DebugOut(L"line 68\n");
		vx = 0;
		SetState(KOOPAS_STATE_DEFEND);
	}
	if (health == 1)
	{
		IsDefending = false;
		if (state != KOOPAS_STATE_ATTACK)
			SetState(KOOPAS_STATE_ATTACK);
		//DebugOut(L"health==1 line 56 \n");
	}
	
	//DebugOut(L"%d\n", state);
	if (IsWalking)
	{
		state = KOOPAS_STATE_WALKING;
	}
	if (IsDefending)
	{
		state = KOOPAS_STATE_DEFEND;
		vx = 0;
		///vy = 0;
		DebugOut(L"%f\n",y);
	}
	if (IsAttacking)
	{
		state = KOOPAS_STATE_ATTACK;
		vx = nx * KOOPAS_ATTACK_SPEED;
		//DebugOut(L"LINE 72 ATTACK STATE\n");
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
			if (state == KOOPAS_STATE_WALKING)
			{

				if (e->obj->GetType() == GType::BRICK) {
					if (e->nx != 0)
					{
						this->nx = -this->nx;
						vx = this->nx * KOOPAS_WALKING_SPEED;
					}
				}
				else {
					x += dx;
				}
			}
			if (state == KOOPAS_STATE_ATTACK)
			{
				if (e->nx != 0) {
					if (e->obj->GetType() == GType::BRICK|| e->obj->GetType() == GType::QUESTIONBRICK || e->obj->GetType() == GType::PIPE) {
						x -= -(this->nx) * 0.04f;
							this->nx = -this->nx;
							this->vx = this->nx * KOOPAS_ATTACK_SPEED;
							//DebugOut(L"%f\n", vx);
						
						
					}
					if (e->obj->GetType() == GType::GOOMBA)
					{
						
							e->obj->SubHealth(1);
						
					}
					else { x += dx; }
				}
			}
			/*else {
				x += dx;
			}*/
			//if (e->obj->GetType() == GType::COLORBRICK)
			//{
			//	if (nx != 0)
			//	{
			//		x += dx;
			//	}
			//	/*if (e->ny == -1)
			//	{
			//		state = KOOPAS_STATE_WALKING;
			//		vx += dx;
			//		vy = 0;
			//	}*/
			//}

		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CKoopas::Render()
{
	if (health==0)
		return;
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DEFEND || state == KOOPAS_STATE_ATTACK) {
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
	case KOOPAS_STATE_DEFEND:


		//DebugOut(L"LINE 147 KOOPAS.CPP KOOPAS STATE DIE \n");
		IsDefending = true;
		break;
	case KOOPAS_STATE_WALKING:
		//this->state = KOOPAS_STATE_WALKING;
		IsWalking = true;
		break;
	case KOOPAS_STATE_ATTACK:
		IsAttacking = true;
		//DebugOut(L"CASE ATTACK \n");
		break;
	}

}
void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DEFEND || state == KOOPAS_STATE_ATTACK)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}
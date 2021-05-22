#include "Koopas.h"
#include "Utils.h"
#include "ColorBrick.h"
#include "QuestionBrick.h"
#include"ShinningBrick.h"
CKoopas::CKoopas(CMario *m, int range)
{
	this->range = range;
	SetState(KOOPAS_STATE_WALKING);
	state = KOOPAS_STATE_WALKING;
	//StartPX = 0;
	nx = 1;
	type = GType::KOOPAS;
	vx = nx * KOOPAS_WALKING_SPEED;
	SetHealth(3);
	IsDefending = IsUp = IsAttacking = IsUpAttacking = false;
	level = KOOPAS_LEVEL_NORMAL;
	IsWalking = true;
	mario = m;
	time = 0;
	IsOnGround = false;
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//DebugOut(L"koopas y:%f\n", y);
	//DebugOut(L"2:%f\n", vx);
	if (health <= 0)
	{
		IsDie = true;
	}
	if (IsDie || IsHeld)
		return;
	//DebugOut(L"speed:%f\n", vx);
	float cam_x = CGame::GetInstance()->GetCamX();
	float cam_w = CGame::GetInstance()->GetScreenWidth();
	//out cam
	if (this->y > 430) {
		SetHealth(0);
	}
	if (IsAttacking || IsUpAttacking) {
		if (x > cam_x + cam_w + 100 || x < (cam_x - 100) || x < 0)
			health = 0;
		//DebugOut(L"OUT CAM LINE 31 KOOPAS\n");
	}

	if (IsWalking) {
		//DebugOut(L"LINE 45\n");
		if (this->x <= (BL + 5) || ((this->x + KOOPAS_BBOX_WIDTH) > (BR - 5)))
		{
			nx = -nx;
			vx = nx * KOOPAS_WALKING_SPEED;
			//DebugOut(L"TURN\n");
		}
		else
		{
			vx = nx * KOOPAS_WALKING_SPEED;
			//DebugOut(L"go\n");
		}
		if (x < 0)
			health = 0;

	}
	if (IsDefending || IsUp)
	{
		//DebugOut(L"line 76 \n");
		//state = KOOPAS_STATE_DEFEND;
		vx = 0;
		if (time == 0)
		{
			time = GetTickCount64();
		}
		if (GetTickCount64() - time > 2000)
		{
			time = 0;
			SetState(KOOPAS_STATE_WALKING);
			this->PlusHealth(1);
			this->y -= 10;
		}
	}

	if (IsAttacking || IsUpAttacking)
	{
		//state = KOOPAS_STATE_ATTACK;
		vx = nx * KOOPAS_ATTACK_SPEED;
		time = 0;
		//DebugOut(L"LINE 72 ATTACK STATE\n");
	}
	CGameObject::Update(dt, coObjects);
	vy += KOOPAS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		//nx = -nx;
		//vx = nx * KOOPAS_WALKING_SPEED;
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;
		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == GType::SHINNINGBRICK)
			{
				CShinningBrick *sbrick = dynamic_cast<CShinningBrick*>(e->obj);
				if (sbrick->GetState() == BRICK_STATE_NORMAL)
				{
					if (e->ny != 0)
					{
						vy = 0;
						y += e->ny * 0.4f;
						bool mostleft = false, mostright = false;
						BL = sbrick->x - 16;
						BR = BL + 45;
						while (!mostleft || !mostright)
						{
							if (!mostleft)
							{

								if (i > 0 && coEventsResult[i - 1]->obj->GetType() == GType::SHINNINGBRICK&&coEventsResult[i - 1]->obj->GetY() == e->obj->GetY())
								{
									BL = BL - 16;
								}
								else
								{
									mostleft = true;
								}
							}
							if (!mostright)
							{

								if (i > 0 && i < coEventsResult.size() - 2 && coEventsResult[i + 1]->obj->GetType() == GType::SHINNINGBRICK&&coEventsResult[i + 1]->obj->GetY() == e->obj->GetY())
								{
									BR = BR + 16;

								}
								else {
									mostright = true;
								}
							}
						}

					}
				}
				/*else {
					x += dx;
					y += dy;
				}*/
			}	
			if (state == KOOPAS_STATE_WALKING)
			{
				if (e->obj->GetType() == GType::COLORBRICK || e->obj->GetType() == GType::GOOMBA)
				{
					//x += dx;
					ColorBrick *colorbrick = dynamic_cast<ColorBrick*>(e->obj);
					if (e->ny != 0)
					{
						BL = colorbrick->GetXOfColorBrick();
						BR = BL + colorbrick->width;
					}
					if (e->nx != 0)
					{
						x += dx;
					}
				}
				
				if (e->obj->GetType() == GType::BRICK) {
					if (e->nx != 0)
					{
						this->nx = -this->nx;
						vx = this->nx * KOOPAS_WALKING_SPEED;
					}
					if (e->ny != 0)
					{
						//y += e->ny*0.4f;
						BL = -99999;
						BR = 99990;
					}
				}

				if (e->obj->GetType() == GType::PIPE)
				{
					this->nx = -this->nx;
					vx = this->nx * KOOPAS_WALKING_SPEED;
				}



			}
			if (state == KOOPAS_STATE_ATTACK || state == KOOPAS_STATE_UP_ATTACK)
			{
				if (e->nx != 0) {
					if (e->obj->GetType() == GType::BRICK || e->obj->GetType() == GType::PIPE) {
						x -= -(this->nx) * 0.04f;
						this->nx = -this->nx;
						this->vx = this->nx * KOOPAS_ATTACK_SPEED;
						//DebugOut(L"line 153\n");


					}
					else if (e->obj->GetType() == GType::QUESTIONBRICK)
					{
						this->nx = -this->nx;
						this->vx = this->nx * KOOPAS_ATTACK_SPEED;
						CQuestionBrick *QBrick = dynamic_cast<CQuestionBrick *>(e->obj);
						if (QBrick->bricktype == QUESTIONBRICK_TYPE_COIN)
						{
							QBrick->HiddenItem = TYPE_COIN;
						}
						else {
							if (mario->level == MARIO_LEVEL_SMALL)
							{
								QBrick->HiddenItem = TYPE_MUSHROOM;
							}
							else
							{
								QBrick->HiddenItem = TYPE_LEAF;
							}
						}
						QBrick->SetState(BRICK_STATE_COLISSION);

					}
					else if (e->obj->GetType() == GType::GOOMBA)
					{

						e->obj->SubHealth(1);
						this->x += dx;

					}
					else if (e->obj->GetType() == GType::SHINNINGBRICK)
					{
						e->obj->SubHealth(1);
						e->obj->SetState(SBRICK_STATE_NOTHINGLEFT);
						//coObjects->at(i)->SetState(SBRICK_STATE_NOTHINGLEFT);
						//e->obj->SetState(SBRICK_STATE_NOTHINGLEFT);
						x -= -(this->nx) * 0.04f;
						this->nx = -this->nx;
						this->vx = this->nx * KOOPAS_ATTACK_SPEED;
					}
					else { x += dx; }
				}
			}

			if (state == KOOPAS_STATE_DIE_UP)
			{

				x += dx;
				y += dy;

			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//DebugOut(L"KOOPAS VY:%f\n", vy);

}

void CKoopas::Render()
{
	if (health == 0)
		return;
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DEFEND) {
		ani = KOOPAS_ANI_DEFEND;
	}
	if (state == KOOPAS_STATE_UP)
	{
		ani = KOOPAS_ANI_DIE;
	}
	if (vx > 0 && state == KOOPAS_STATE_WALKING)
	{
		ani = KOOPAS_ANI_WALKING_RIGHT;
	}
	if (vx < 0 && state == KOOPAS_STATE_WALKING) {
		ani = KOOPAS_ANI_WALKING_LEFT;
		//ani = KOOPAS_ANI_WALKING_RIGHT;
	}
	if (state == KOOPAS_STATE_ATTACK)
	{
		ani = KOOPAS_ANI_ATTACK;
	}
	if (state == KOOPAS_STATE_UP_ATTACK)
	{
		ani = KOOPAS_ANI_DIE_ATTACK;
	}
	if (state == KOOPAS_STATE_DIE_UP) {
		ani = KOOPAS_ANI_DIE;
	}
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
		IsWalking = false;
		IsAttacking = false;
		IsUpAttacking = false;
		IsUp = false;
		this->state = KOOPAS_STATE_DEFEND;
		break;
	case KOOPAS_STATE_WALKING:
		this->state = KOOPAS_STATE_WALKING;
		IsWalking = true;
		IsDefending = false;
		IsAttacking = false;
		IsUpAttacking = false;
		IsUp = false;
		//vx = nx * KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_ATTACK:
		IsDefending = false;
		IsWalking = false;
		IsAttacking = true;
		IsUp = false;
		IsUpAttacking = false;
		this->state = KOOPAS_STATE_ATTACK;
		//IsUpAttacking = false;
		//IsUp = false;
		//DebugOut(L"CASE ATTACK \n");
		break;
	case KOOPAS_STATE_UP:
		//DebugOut(L"line ");
		this->vy = -0.9f;
		IsDefending = false;
		IsWalking = false;
		IsAttacking = false;
		IsUpAttacking = false;
		IsUp = true;
		this->state = KOOPAS_STATE_UP;
		vx = 0;
		//this->state = KOOPAS_STATE_UP;
		break;
	case KOOPAS_STATE_UP_ATTACK:
		this->state = KOOPAS_STATE_UP_ATTACK;
		IsDefending = false;
		IsWalking = false;
		IsAttacking = false;
		IsUpAttacking = true;
		IsUp = false;
		//DebugOut
		break;
	case KOOPAS_STATE_DIE_UP:
	{
		this->state = KOOPAS_STATE_DIE_UP;
		vy = -KOOPAS_DIE_UP_DEFLECT;
		IsDefending = false;
		IsUp = false;
		IsWalking = false;
		IsAttacking = IsUpAttacking = false;
		IsDieUp = true;
		break;
	}
	}

}
void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DEFEND || state == KOOPAS_STATE_ATTACK || state == KOOPAS_STATE_UP || state == KOOPAS_STATE_UP_ATTACK)
	{
		top = y;
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;

	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}
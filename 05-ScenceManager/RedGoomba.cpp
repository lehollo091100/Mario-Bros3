#include "RedGoomba.h"
RedGoomba::RedGoomba()
{
	type = GType::REDGOOMBA;
	//SetHealth(2);
	SetState(REDGOOMBA_STATE_ONGROUND);
	nx = -1;
	vx = nx * REDGOOMBA_VX;
	IsOnGround = true;
	NoJump = NoJumpHigh = 0;
	SetHealth(3);
	time = 0;
	IsNoWings = false;
	IsJump = false;
}

void RedGoomba::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	if (health == 2) {
		right = left + REDGOOMBA_SMALL_BBOX_HEIGTH;
		bottom = top + REDGOOMBA_SMALL_BBOX_WIDTH;
	}
	else
	{
		if (IsJump)
		{
			right = left + REDGOOMBA_BIG_BBOX_HEIGHT;
			bottom = top + REDGOOMBA_BIG_BBOX_WIDTH;
		}
		else {
			right = left + REDGOOMBA_SMALL_BBOX_HEIGTH;
			bottom = top + REDGOOMBA_SMALL_BBOX_WIDTH;
		}

	}


}

void RedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += REDGOOMBA_GRAVITY * dt;
	vx = nx * REDGOOMBA_VX;
	if (health == 0)
		IsDie = true;
	if (IsDie)
		return;
	if (startY == 0)
		startY = y;
	if (health == 3)
	{
		if (IsOnGround)
		{
			if (timeonground == 0)
				timeonground = GetTickCount64();
			if (GetTickCount64() - timeonground >= 2000)
			{
				timeonground = 0;
				IsOnGround = false;
				vy = -REDGOOMBA_DEFLECT_SPEED_LOW;
				SetState(REDGOOMBA_STATE_JUMP);
			}
		}

	}
	if (health == 2)
	{
		SetState(REDGOOMBA_STATE_NOWINGS);
	}
	if (health == 1)
	{

		if (state == REDGOOMBA_STATE_DIE)
		{
			vx = 0;
			if (time <= 30)
			{
				time++;
			}
			else {
				time = 0;
				SubHealth(1);
			}
		}
		if (state == REDGOOMBA_STATE_DIE_UP)
		{
			

		}
	}
	if (this->y > 600)
	{
		SubHealth(1);
	}
	//DebugOut(L"state1:%d\n", state);
	/*if (NoJump == 4)
	{

		NoJump = 0;
		IsJump = false;
		SetState(REDGOOMBA_STATE_ONGROUND);
	}*/
	/*if (NoJumpHigh == 1)
	{
		IsJump = false;
		NoJumpHigh = 0;
		SetState(REDGOOMBA_STATE_ONGROUND);
	}*/
	//DebugOut(L"health:%d\n", this->health);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	//coEventsResult.clear();
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
		if (IsOnGround||IsNoWings)
			if (ny < 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			/*if (IsReady)
			{
				if (e->obj->GetType() == GType::BRICK)
				{
					if (ny < 0)
					{
						vy = -REDGOOMBA_DEFLECT_SPEED_LOW;
						NoJump += 1;
					}
				}
			}*/
			if (e->nx != 0)
			{

				if (e->obj->GetType() == GType::QUESTIONBRICK)
				{
					if (!IsOnGround)
					{
					this->nx = -this->nx;

					vx = this->nx * REDGOOMBA_VX;
					}
					if (IsNoWings) {
						/*this->nx = -this->nx;
						vx = this->nx * REDGOOMBA_VX;*/
					}
					else if (IsOnGround)
					{
						this->nx = -this->nx;
						vx = this->nx * REDGOOMBA_VX;
					}
				}
				if (e->obj->GetType() == GType::GOOMBA)
				{
					x += dx;
					//y += dy;
				}
			}
			if (e->ny != 0)
			{
				if (Isdieup)
				{
					y += dy;
					x += dx;
				}
				if (e->obj->GetType() == GType::BRICK)
				{
					if (IsJump)
					{
						if (ny < 0)
						{
							NoJump++;
							if (NoJump < 3)
							{

								vy = -REDGOOMBA_DEFLECT_SPEED_LOW;

							}
							if (NoJump == 4)
							{
								//DebugOut(L"state:%d\n", state);
								SetState(REDGOOMBA_STATE_ONGROUND);
								IsJump = false;
								NoJump = 0;

							}

							if (NoJump == 3)
							{
								//DebugOut(L"state:%d\n", state);
								//this->y -= 5;
								vy = -REDGOOMBA_DEFLECT_SPEED_HIGH;

							}

							//IsJump = false;
						}
					}
					
				}


			}


			/*if (e->obj->GetType() == GType::BRICK)
			{
				if (e->nx != 0)
				{
					this->nx = -this->nx;
					vx = nx * REDGOOMBA_VX;
				}
			}*/


		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (startY > this->y)
		IsReady = true;
	else
		IsReady = false;
}

void RedGoomba::Render()
{
	if (IsDie)
		return;
	int ani = 0;
	if (state == REDGOOMBA_STATE_NOWINGS)
	{
		ani = REDGOOMBA_ANI_NOWINGS;
	}
	if (state == REDGOOMBA_STATE_ONGROUND)
	{
		ani = REDGOOMBA_ANI_MOVING;
	}
	if (state == REDGOOMBA_STATE_JUMP)
	{
		//	DebugOut(L"line 147 redgoomba\n");
		ani = REDGOOMBA_ANI_JUMP;
	}
	if (state == REDGOOMBA_STATE_READY)
	{
		ani = REDGOOMBA_ANI_JUMP;
	}
	if(state==REDGOOMBA_STATE_DIE_UP)
	{
		ani = REDGOOMBA_ANI_DIE_UP;
	}
	if (state == REDGOOMBA_STATE_DIE)
	{
		ani = REDGOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void RedGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case REDGOOMBA_STATE_ONGROUND:
	{
		this->state = REDGOOMBA_STATE_ONGROUND;
		IsOnGround = true;
		//vx = nx * FLYKOOPAS_WALKING_SPEED;
		break;
	}
	case REDGOOMBA_STATE_JUMP:
	{
		IsJump = true;
		this->state = REDGOOMBA_STATE_JUMP;
		break;
	}
	case REDGOOMBA_STATE_NOWINGS:
	{
		IsNoWings = true;
		IsOnGround = false;
		IsJump = false;
		this->state = REDGOOMBA_STATE_NOWINGS;
		break;
	}
	case REDGOOMBA_STATE_DIE:
	{
		vx = 0;
		Isdie = true;
		IsNoWings = false;
		this->state = REDGOOMBA_STATE_DIE;
		break;
	}
	case REDGOOMBA_STATE_DIE_UP:
	{
		vx = 0;
		vy = -REDGOOMBA_DEFLECT_SPEED_HIGH;
		Isdieup = true;
		IsNoWings = false;
		IsOnGround = false;
		this->state = REDGOOMBA_STATE_DIE_UP;
		break;
	}
	}
}


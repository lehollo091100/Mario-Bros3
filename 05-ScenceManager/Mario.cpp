#include <algorithm>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Brick.h"
#include "Portal.h"
#include "Sprites.h"
#include "Koopas.h"

CMario::CMario(float x, float y) : CGameObject()
{
	type = GType::MARIO;
	level = MARIO_LEVEL_BIG;
	IsWalking = false;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	Gravity = 0;
	IsWalkingR = IsWalkingL = IsJumping = IsSlowDown = IsMaxspeed = IsSitting = IsDie = false;
	IsFlying = false;
	Fire *f = new Fire(1);
	LstWeapon.push_back(f);
	Fire *f1 = new Fire(1);
	LstWeapon.push_back(f1);
	IsOnGround = true;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	/*if (state == MARIO_STATE_SIT)
		DebugOut(L"line 39 state sit\n");*/
	if (x < CGame::GetInstance()->GetCamX())
		x = CGame::GetInstance()->GetCamX();
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (vy <= 0)
		vy += MARIO_GRAVITY * dt;
	else
		if (IsJumping || IsFlying)
		{
			if (vy > 0) {
				Gravity += 0.00011f;
				vy += Gravity * dt;
			}
		}
		else
			vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	DWORD now = GetTickCount64();
	time = now - startfly;
	if (IsAttacking && now - LastimeAttack > 470 && level == MARIO_LEVEL_TAIL)
	{
		IsAttacking = false;
		state = MARIO_STATE_IDLE;
	}
	if (IsAttacking && now - LastimeAttack > 240 && level == MARIO_LEVEL_FIRE)
	{
		IsAttacking = false;
		state = MARIO_STATE_IDLE;
	}
	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	/*if (!IsWalking)
		vx -= nx*MARIO_DECELERATE;*/
	if (IsWalking) {
		//IsSitting = false;
		if (vx == 0) {
			vx = nx * MARIO_WALKING_SPEED;
			if (nx > 0)
				state = MARIO_STATE_WALKING_RIGHT;
			else
				state = MARIO_STATE_WALKING_LEFT;
		}
		else
			if (vx > 0)
			{
				//DebugOut(L"vx > 0\n");
				if (IsWalkingR) {
					//DebugOut(L"WR\n");
					vx += MARIO_ACCELERATE;
					state = MARIO_STATE_WALKING_RIGHT;
					if (!IsRunning && vx >= MARIO_WALKING_MAXSPEED)
						vx = MARIO_WALKING_MAXSPEED;
					else if (IsRunning) {
						if (vx >= 0.2f)
						{
							vx = MARIO_RUNNING_MAXSPEED;
							state = MARIO_STATE_RUN_MAXSPEED;
						}
						else
							state = MARIO_STATE_RUNNING;
					}
				}
				else {
					if (IsWalkingL) {
						//ROLLBACK
						vx -= MARIO_DECELERATE;
						if (vx >= 0) {
							state = MARIO_STATE_TURN;
						}
						else {
							vx = 0;
						}
					}
					else if (IsOnGround)
					{
						//SLOWDOWN
						//DebugOut(L"line 113 mario Slow speed %f\n", vx);
						vx -= MARIO_ACCELERATE;
						if (vx <= 0)
						{
							vx = 0;
							IsWalking = false;
							if (!IsSitting) {
								DebugOut(L"line 126 IDLE\n");
								state = MARIO_STATE_IDLE;
							}
						}
					}
				}
			}
			else {////vx<0------->dang di qua trai/////////////
				if (IsWalkingL) {
					vx -= MARIO_ACCELERATE;
					state = MARIO_STATE_WALKING_LEFT;
					if (!IsRunning &&vx <= -MARIO_WALKING_MAXSPEED)
						vx = -MARIO_WALKING_MAXSPEED;
					else if (IsRunning) {
						if (vx <= -0.2f) {
							vx = -0.2f;
							state = MARIO_STATE_RUN_MAXSPEED;
						}
						else
							state = MARIO_STATE_RUNNING;
					}

				}
				else {
					if (IsWalkingR) {
						//ROLLBACK
						vx += MARIO_DECELERATE;
						if (vx <= 0) {
							state = MARIO_STATE_TURN;
						}
						else {
							vx = 0;
						}
					}
					else if (IsOnGround) {
						//SLOWDOWN
						vx += MARIO_ACCELERATE;
						if (vx >= 0)
						{
							vx = 0;
							IsWalking = false;
							if (!IsSitting) {
								DebugOut(L"line 167 IDLE\n");
								state = MARIO_STATE_IDLE;
							}
						}
					}
				}
			}
	}

	if (IsFlying) {
		state = MARIO_STATE_FLY;
		if (vy >= 0)
			Gravity = 0;
	}
	if (IsJumping) {
		state = MARIO_STATE_JUMP;
		if (vy >= 0) {
			if (!IsFalling)
			{
				Gravity = 0;
				IsFalling = true;
			}
			state = MARIO_STATE_FALLING;
		}
	}
	if (IsFallfly&&level != 4)
	{
		state = MARIO_STATE_FALLINFLY;
	}
	if (IsFalling&&level != 4)
	{
		state = MARIO_STATE_FALLING;
	}
	if (IsFalling&&level == MARIO_LEVEL_TAIL) {
		state = MARIO_STATE_FALLING;
		if (IsFlying) {
			state = MARIO_STATE_FALLINFLY;
			if (!IsFlyup)
				state = MARIO_STATE_FALLING;
		}
	}
	if (IsFallSlow) {
		state = MARIO_STATE_FALLING_SLOW;
	}
	if (IsAttacking&&level == MARIO_LEVEL_TAIL)
		state = MARIO_STATE_ATTACK_TAIL;
	else if (IsAttacking&&level == MARIO_LEVEL_FIRE)
	{
		state = MARIO_STATE_FIRE;
	}
	if (IsSitting)
	{
		DebugOut(L"line 219 Sitting\n");
		state = MARIO_STATE_SIT;
	}

	if (IsDie)
	{
		state = MARIO_STATE_DIE;
	}
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		//IsOnGround = false;
		//DebugOut(L"No col\n");
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		int posObjx = 0;
		int posObjy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.6f;
		y += min_ty * dy + ny * 0.4f;

		if (min_tx != 1 && min_ty != 1 && coEventsResult[1]->obj->GetType() == GType::BRICK) {
			DebugOut(L"line 247 coevent = brick\n");
			IsOnGround = true;
		}
		/*if (nx != 0) {
			if (!IsWalking)
				state = MARIO_STATE_IDLE;
		}*/
		//if (ny != 0) vy = 0;
		if (ny == -1) if (!IsJumping && !IsFlying) { vy = 0; DebugOut(L"Oh no \n"); }
		// Collision logic with other objects
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->nx != 0) {
				DebugOut(L"%d\n", nx);
				//colorbrick
				if (e->obj->GetType() == GType::COLORBRICK) { x += dx; }

				//brick
				if (e->obj->GetType() == GType::BRICK) {
					if (!IsWalking)
						if (!IsSitting) {
							//DebugOut(L"line 277 IDLE\n");
							state = MARIO_STATE_IDLE;
						}
						else
						{
							state = MARIO_STATE_SIT;
						}
					if (!IsFlying) {
						IsOnGround = true;

					}
				}
				//KOOPAS
				if (e->obj->GetType() == GType::KOOPAS)
				{
					//if (e->nx != 0) {

						if (e->obj->GetState() == KOOPAS_STATE_WALKING)
						{
							//DebugOut(L"LINE 296 DIE NX!=0");
							SetState(MARIO_STATE_DIE);
						}
						if (e->obj->GetState() == KOOPAS_STATE_DEFEND)
						{
							e->obj->SubHealth(1);
						}
					//}
				}
			}
			/*else if(nx==0){
				if (e->obj->GetType() == GType::KOOPAS)
					if (e->nx != 0&&e->obj->GetState()==KOOPAS_STATE_WALKING)
						SetState(MARIO_STATE_DIE);
			}*/
			if (e->obj->GetType() == GType::KOOPAS)
			{
				DebugOut(L"e->ny%f\n", e->ny);
			}
			if (e->ny != 0) {
				if (e->obj->GetType() == GType::COLORBRICK) {
					if (e->ny == 1) {
						y += dy;
						//KO XAY RA CHUYEN GI
					}
					else {
						IsOnGround = true;
						if (IsFalling) {
							IsFalling = false;
							if (vx != 0) {
								state = MARIO_STATE_WALKING;
							}
							else
								if (!IsSitting) {
									DebugOut(L"line 302 IDLE\n");
									state = MARIO_STATE_IDLE;
								}
						}
						if (IsFallSlow)
						{
							IsFallSlow = false;
							vx = nx * MARIO_WALKING_SPEED;
							state = MARIO_STATE_WALKING;
						}
						if (IsFlyup)
							IsFlyup = false;
						IsJumping = false;
						IsFlying = false;
						if (IsFallfly)
						{
							IsWalking = true;
							IsFallfly = false;
							state = MARIO_STATE_WALKING;
							vx = nx * MARIO_WALKING_SPEED;
						}
					}
				}
				if (e->obj->GetType() == GType::BRICK) {
					if (e->ny == 1) {
						if (vy < 0) {
							Gravity = 0;
							vy += 0.005f;
						}
						if (IsJumping) {
							IsJumping = false;
							IsFalling = true;
							/*if (!IsSitting)
								DebugOut(L"mario line 323 jump\n");*/
							state = MARIO_STATE_FALLING;
						}
					}
					else {
						IsOnGround = true;
						if (IsFalling) {
							IsFalling = false;
							if (vx != 0) {
								state = MARIO_STATE_WALKING;
							}
							else
								if (!IsSitting) {
									DebugOut(L"line 348 IDLE\n");
									state = MARIO_STATE_IDLE;
								}
						}
						if (IsFallSlow)
						{
							IsFallSlow = false;
							vx = nx * MARIO_WALKING_SPEED;
							state = MARIO_STATE_WALKING;
						}
						if (IsFlyup)
							IsFlyup = false;
						IsJumping = false;
						IsFlying = false;
						if (IsFallfly)
						{
							IsWalking = true;
							state = MARIO_STATE_WALKING;
							vx = nx * MARIO_WALKING_SPEED;
							IsFallfly = false;
						}
					}
				}
				if (e->obj->GetType() == GType::KOOPAS)
				{
					DebugOut(L"e->ny%f\n", e->ny);
					if (e->ny < 0 )
					{
						DebugOut(L"line 407");
						if (e->obj->GetState() == KOOPAS_STATE_WALKING)
						{
							DebugOut(L"Line 411 set state walking");
							e->obj->SubHealth(1);
							vy -= MARIO_DIE_DEFLECT_SPEED;
						}
						if (e->obj->GetState() == KOOPAS_STATE_DEFEND) {
							/*if (x >= e->obj->GetX() + (KOOPAS_BBOX_WIDTH / 2))
							{
								e->nx = -1;
							}
							else if (x < e->obj->GetX() + (KOOPAS_BBOX_WIDTH / 2))
							{
								e->nx = 1;
							}*/
							e->obj->SubHealth(1);
							DebugOut(L"Line 404 set state attack");
							vy -= MARIO_DIE_DEFLECT_SPEED;
							//SweptAABBEx(e->obj);
						}
					}
					else if (e->ny > 0)
					{
						if (e->obj->GetState() == KOOPAS_STATE_WALKING)
						{
							//DebugOut(L"Line 398 set state walking");
							SetState(MARIO_STATE_DIE);
						}
						else
						{
							return;
						}
					}
				}
			}
			else {
				if (e->obj->GetType() == GType::BRICK) {
					IsOnGround = false;
				}
			}
		}
	}

	for (int i = 0; i < LstWeapon.size(); i++) {
		LstWeapon[i]->Update(dt, coObjects);
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (y > 680)
		Reset();
	/*if(state == MARIO_STATE_SIT)
		DebugOut(L"line 387 state sit\n");*/
}

void CMario::Render()
{
	//DebugOut(L"state %d\n", state);
	if (state == MARIO_STATE_DIE) {
		ani = MARIO_ANI_DIE;
	}
	else
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0)
				ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else
				ani = MARIO_ANI_BIG_IDLE_LEFT;
			if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_WALKLEFT_TURNRIGHT;
				else
				{
					ani = MARIO_ANI_BIG_WALKRIGHT_TURNLEFT;
				}
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIO_ANI_RUN_MAXSPEEDRIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RUN_MAXSPEEDLEFT;
			}
			if (state == MARIO_STATE_RUNNING)
			{
				if (nx > 0)
					ani = MARIO_ANI_RUNRIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RUNLEFT;
			}
			if (state == MARIO_STATE_ATTACK_TAIL)
			{
				if (nx > 0)
					ani = MARIOTAIL_ANI_ATTACKRIGHT;
				else if (nx < 0)
					ani = MARIOTAIL_ANI_ATTACKLEFT;
			}
			if (state == MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_JUMP_LEFT;
			}
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_FALL_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_FALL_LEFT;
			}
			if (state == MARIO_STATE_FALLINFLY)
			{
				DebugOut(L"FALLFLY ANI");
				if (nx > 0)
					ani = MARIOBIG_ANI_FLYRIGHT;
				else if (nx < 0)
					ani = MARIOBIG_ANI_FLYLEFT;
			}
			if (state == MARIO_STATE_FLY)
			{
				if (nx > 0)
					ani = MARIOBIG_ANI_FLYRIGHT;
				else if (nx < 0)
					ani = MARIOBIG_ANI_FLYLEFT;
			}
			if (state == MARIO_STATE_SIT)
			{
				if (nx > 0)
					ani = MARIO_ANI_SIT_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SIT_LEFT;
			}

		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (state != MARIO_STATE_ATTACK_TAIL && nx == -1 && ani == MARIOTAIL_ANI_ATTACKLEFT) {
				animation_set->at(ani)->Reset();
				ani = MARIOTAIL_ANI_IDLE_LEFT;
			}
			else if (state != MARIO_STATE_ATTACK_TAIL && nx == 1 && ani == MARIOTAIL_ANI_ATTACKRIGHT)
			{
				animation_set->at(ani)->Reset();
				ani = MARIOTAIL_ANI_IDLE_RIGHT;
			}
			if (nx > 0)
				ani = MARIOTAIL_ANI_IDLE_RIGHT;
			else
				ani = MARIOTAIL_ANI_IDLE_LEFT;
			if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIOTAIL_ANI_WALK_RIGHT;
			if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIOTAIL_ANI_WALK_LEFT;
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIOTAIL_ANI_WALKLEFT_TURNRIGHT;
				else
				{
					ani = MARIOTAIL_ANI_WALKRIGHT_TURNLEFT;
				}
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIOTAIL_ANI_RUN_MAXSPEEDRIGHT;
				else if (nx < 0)
					ani = MARIOTAIL_ANI_RUN_MAXSPEEDLEFT;
			}
			if (state == MARIO_STATE_RUNNING)
			{
				if (nx > 0)
					ani = MARIOTAIL_ANI_WALK_RIGHT;
				else if (nx < 0)
					ani = MARIOTAIL_ANI_WALK_LEFT;
			}
			if (state == MARIO_STATE_ATTACK_TAIL)
			{
				if (nx > 0) {
					ani = MARIOTAIL_ANI_ATTACKRIGHT;
				}
				else
					ani = MARIOTAIL_ANI_ATTACKLEFT;
			}
			if (state == MARIO_STATE_FLY) {
				if (nx > 0) {
					ani = MARIOTAIL_ANI_FLYRIGHT;
					//DebugOut(L"flyani");
				}
				else
					ani = MARIOTAIL_ANI_FLYLEFT;
			}
			if (state == MARIO_STATE_JUMP) {
				if (nx > 0) {
					ani = MARIOTAIL_ANI_JUMP_RIGHT;
				}
				else
					ani = MARIOTAIL_ANI_JUMP_LEFT;
			}
			if (state == MARIO_STATE_FALLING) {
				if (nx > 0) {
					ani = MARIOTAIL_ANI_FALLRIGHT;
				}
				else
					ani = MARIOTAIL_ANI_FALLLEFT;
			}
			if (state == MARIO_STATE_FALLING_SLOW) {
				if (nx > 0) {
					ani = MARIOTAIL_ANI_FALLSLOWRIGHT;
				}
				else
					ani = MARIOTAIL_ANI_FALLSLOWLEFT;
			}
			if (state == MARIO_STATE_FALLINFLY)
			{
				if (nx > 0) {
					ani = MARIOTAIL_ANI_FALLFLY_RIGHT;
				}
				else
					ani = MARIOTAIL_ANI_FALLFLY_LEFT;
			}
			if (state == MARIO_STATE_SIT)
			{
				if (nx > 0) {
					ani = MARIOTAIL_ANI_SIT_RIGHT;
				}
				else
					ani = MARIOTAIL_ANI_SIT_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0)
				ani = MARIOWHITE_ANI_IDLE_RIGHT;
			else
				ani = MARIOWHITE_ANI_IDLE_LEFT;
			if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIOWHITE_ANI_WALK_RIGHT;
			if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIOWHITE_ANI_WALK_LEFT;
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIOWHITE_ANI_WALKLEFT_TURNRIGHT;
				else
				{
					ani = MARIOWHITE_ANI_WALKRIGHT_TURNLEFT;
				}
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIOWHITE_ANI_RUN_MAXSPEEDRIGHT;
				else if (nx < 0)
					ani = MARIOWHITE_ANI_RUN_MAXSPEEDLEFT;
			}
			if (state == MARIO_STATE_RUNNING)
			{
				if (nx > 0)
					ani = MARIOWHITE_ANI_RUN_RIGHT;
				else if (nx < 0)
					ani = MARIOWHITE_ANI_RUN_LEFT;
			}
			if (state == MARIO_STATE_FIRE)
			{
				if (nx > 0)
				{
					ani = MARIOWHITE_ANI_SHOOT_FIRE_RIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIOWHITE_ANI_SHOOT_FIRE_LEFT;
				}
			}
			if (state == MARIO_STATE_JUMP)
			{
				if (nx > 0)
				{
					ani = MARIOWHITE_ANI_JUMP_RIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIOWHITE_ANI_JUMP_LEFT;
				}
			}
			if (state == MARIO_STATE_FLY)
			{
				if (nx > 0) {
					ani = MARIOWHITE_ANI_FLYRIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIOWHITE_ANI_FLYLEFT;
				}
			}
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0) {
					ani = MARIOWHITE_ANI_FALLRIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIOWHITE_ANI_FALLLEFT;
				}
			}
			if (state == MARIO_STATE_FALLINFLY)
			{
				if (nx > 0) {
					ani = MARIOWHITE_ANI_FLYRIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIOWHITE_ANI_FLYLEFT;
				}
			}
			if (state == MARIO_STATE_SIT)
			{
				if (nx > 0) {
					ani = MARIOWHITE_ANI_SIT_RIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIOWHITE_ANI_SIT_LEFT;
				}
			}

		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0)
				ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else
				ani = MARIO_ANI_SMALL_IDLE_LEFT;
			if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_SMALL_WALKING_LEFT;
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_WALKLEFT_TURNRIGHT;
				else
				{
					ani = MARIO_ANI_SMALL_WALKRIGHT_TURNLEFT;
				}
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIOSMALL_ANI_RUN_RIGHT;
				else if (nx < 0)
					ani = MARIOSMALL_ANI_RUN_LEFT;
			}
			if (state == MARIO_STATE_RUNNING)
			{
				if (nx > 0)
					ani = MARIOSMALL_ANI_RUN_RIGHT;
				else if (nx < 0)
					ani = MARIOSMALL_ANI_RUN_LEFT;
			}
			/*if (state == MARIO_STATE_ATTACK_TAIL)
			{
				if (nx > 0)
					ani = MARIOTAIL_ANI_ATTACKRIGHT;
				else if (nx < 0)
					ani = MARIOTAIL_ANI_ATTACKLEFT;
			}*/
			if (state == MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
			if (state == MARIO_STATE_FLY)
			{
				if (nx > 0)
					ani = MARIOSMALL_ANI_FLYRIGHT;
				else if (nx < 0)
					ani = MARIOSMALL_ANI_FLYLEFT;
			}

		}

	int alpha = 255;
	if (untouchable) alpha = 128;
	//if (nx == 1)
	animation_set->at(ani)->Render(x, y, alpha);
	for (int i = 0; i < LstWeapon.size(); i++) {
		LstWeapon[i]->Render();
	}
	RenderBoundingBox();
}


void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		IsWalking = true;
		IsWalkingR = true;
		if (IsSitting)
		{
			IsSitting = false;
			y -= 10;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		IsWalking = true;
		IsWalkingL = true;
		if (IsSitting)
		{
			IsSitting = false;
			y -= 10;
		}
		//IsSitting = false;
		break;
	case MARIO_STATE_SPEEDDOWNRIGHT:
		//IsWalking = false;
		IsSlowDown = true;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		//DebugOut(L"line 760 IDLE\n");
		break;
	case MARIO_STATE_RUNNING:
		IsRunning = true;
		vx += nx * 0.00045f;
		break;
	case MARIO_STATE_ATTACK_TAIL:
		this->state = MARIO_STATE_ATTACK_TAIL;
		IsAttacking = true;
		break;
	case MARIO_STATE_FIRE:
		if (nx == 1)
		{
			if (LstWeapon[0]->GetHealth() <= 0)
			{
				LstWeapon[0]->SetHealth(1);
				LstWeapon[0]->SetPosition(x + 10, y + 5);
				LstWeapon[0]->SetDirection(nx);
				LastimeAttack = GetTickCount64();
				this->state = MARIO_STATE_FIRE;
				IsAttacking = true;
			}
			else if (LstWeapon[1]->GetHealth() <= 0)
			{
				LstWeapon[1]->SetHealth(1);
				LstWeapon[1]->SetPosition(x + 10, y + 5);
				LstWeapon[1]->SetDirection(nx);
				this->state = MARIO_STATE_FIRE;
				LastimeAttack = GetTickCount64();
				IsAttacking = true;
			}

		}
		else {
			if (LstWeapon[0]->GetHealth() <= 0)
			{
				LstWeapon[0]->SetHealth(1);
				LstWeapon[0]->SetPosition(x, y + 5);
				LstWeapon[0]->SetDirection(nx);
				this->state = MARIO_STATE_FIRE;
				LastimeAttack = GetTickCount64();
				IsAttacking = true;
				//DebugOut(L"%d\n", nx);
			}
			else if (LstWeapon[1]->GetHealth() <= 0)
			{
				LstWeapon[1]->SetHealth(1);
				LstWeapon[1]->SetPosition(x, y + 5);
				LstWeapon[1]->SetDirection(nx);
				this->state = MARIO_STATE_FIRE;
				LastimeAttack = GetTickCount64();
				IsAttacking = true;
			}
		}break;
	case MARIO_STATE_SIT:
		if ((!IsWalkingR&&IsWalkingL == false && !IsRunning) || vx == 0)
		{
			IsSitting = true;
		}
		else
		{
			IsSitting = false;
		}
		break;
	case MARIO_STATE_UP:
	{
		//DebugOut(L"line 787 Mario : Up\n");
		if (IsOnGround) {
			//DebugOut(L"line 789 Mario : On ground\n");
			startfly = GetTickCount64();
			if ((nx < 0 && vx > -MARIO_RUNNING_MAXSPEED) || (nx > 0 && vx < MARIO_RUNNING_MAXSPEED))
			{
				//jump
				//DebugOut(L"line 742 Mario : Jump press\n");
				IsJumping = true;
				IsOnGround = false;
				vy = -MARIO_JUMP_SPEED_Y;
			}
			else {
				//fly
				IsFlying = true;
				IsFlyup = true;
				IsOnGround = false;
				vy = -MARIO_JUMP_SPEED_Y;

			}
		}
		else {
			//falling
			//DebugOut(L"line 808 Mario : Falling\n");
			if (!IsJumping && !IsFlying) {
				vy = -MARIO_JUMP_SPEED_Y;
				IsJumping = true;
				break;
			}
			if (level == 4) {
				if (IsJumping) {
					if (vy > 0) {
						if (!IsFallSlow) {
							Gravity = -0.0006f;
							IsFallSlow = true;
						}
					}
				}
				if (IsFlying)
				{
					//DebugOut(L"fly\n");
					if (time <= 3000)
					{

						//IsFlying = true;
						IsFalling = false;
						Gravity = -0.0095f;
						vx -= nx * 0.02f;
						IsFlyup = true;
						//this->state = MARIO_STATE_FLY;
					}
					else
					{
						//IsFlying = false;
						Gravity = -0.00075f;
						IsFlyup = false;
						IsFallSlow = true;
						//DebugOut(L"Fly fall slow");
						vx -= 0.02f;
						if (vx <= 0.03f)
							vx = 0.03f;
						//IsFalling = true;
					}
				}

			}
			if (level == 2 || level == 3 || level == 1)
			{
				if (IsJumping) {
					if (IsFalling) {}
					//DebugOut(L"KO JUMP NUA\n");
					//IsJumping = false;
				}
				if (IsFlying)
				{
					/*if (IsFallfly)
						IsFlying = false;*/
				}

			}
		}
		//DebugOut(L"FG: %f\n", Gravity);
		break;
	}
	case MARIO_STATE_DIE:
		if (!IsDie)
		{
			vy = -MARIO_DIE_DEFLECT_SPEED / 2;
			IsDie = true;

		}
		else
		{
			//IsDie = false;
			//this->state = MARIO_STATE_IDLE;
		}
		break;
	default:
		break;
	}

}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_TAIL || level == MARIO_LEVEL_FIRE)
	{

		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (state == MARIO_STATE_SIT)
		{
			right = x + 10;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT + 2;
		}

	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	IsDie = false;
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

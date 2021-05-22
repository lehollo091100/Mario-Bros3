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
#include "QuestionBrick.h"
#include "FlyKoopas.h"
#include "ShinningBrick.h"
#include "Item.h"
#include "Leaf.h"
#include"RedGoomba.h"
#include"EndGameObject.h"
#include"Node.h"
#include"PBrick.h"
#include"PItem.h"
#include"Pipe.h"
#include"UpDownWood.h"
#include"ShinningExtraBrick.h"
#include"FlyUpDownKoopas.h"

CMario::CMario(float x, float y) : CGameObject()
{
	type = GType::MARIO;
	level = MARIO_LEVEL_SMALL;
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
	koo = new CKoopas(NULL);
	time1 = 0;
	IsPortal = false;
	IsMoving = false;
	stack = Coin = 0;
	Point = 0000000;
	IsEndGame = false;
	OutPipeY = 0;
	IsOuttingPipe = IsInPipe = CanPiping = false;
	z = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	/*if (state == MARIO_STATE_SIT)
		DebugOut(L"line 39 state sit\n");*/
		//DebugOut(L"vx%f\n",vx);
	if (x <= 0)
		x = 0;
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	coEventsResult.clear();


	if (IsInMap3 == false && IsInMap4 == false)
	{
		if (IsEndGame)
		{
			vx = MARIO_WALKING_SPEED;
			state = MARIO_STATE_WALKING;
		}
		if (IsWalking)
			stack = 0;
		if (IsRunning)
		{
			if (vx > 0 && vx < 0.04f || (vx<0 && vx>-0.04f))
				stack = 1;
			else if (vx >= 0.04f&&vx < 0.08f || (vx <= -0.04f && vx > -0.08f))
				stack = 2;
			else if (vx >= 0.08f && vx < 0.12f || (vx <= -0.08f && vx > -0.12f))
				stack = 3;
			else if (vx >= 0.12f &&vx < 0.16f || (vx <= -0.12f && vx > -0.16f))
				stack = 4;
			else if (vx >= 0.16f&&vx < 0.2f || (vx <= -0.16 && vx > -0.2f))
				stack = 5;
			else if (vx >= 0.2f || (vx <= -0.2f))
			{
				stack = 6;
			}
		}
		if (vy <= 0)
			vy += MARIO_GRAVITY * dt;
		else
			if (IsJumping || IsFlying)
			{
				if (vy > 0) {
					Gravity += MARIO_GRAVITY_INCREASE;
					vy += Gravity * dt;
				}
			}
			else
				vy += MARIO_GRAVITY * dt;
		DWORD now = GetTickCount64();
		time = now - startfly;
		
		//DebugOut(L"TIME:%d\n",time);
		if (IsAttacking && now - LastimeAttack > 470 && level == MARIO_LEVEL_TAIL)
		{
			/*if(IsOnGround)
				y -= 16;*/
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
						if (!IsRunning) {
							vx += MARIO_ACCELERATE;
							state = MARIO_STATE_WALKING_RIGHT;
							if (!IsRunning && vx >= MARIO_WALKING_MAXSPEED)
								vx = MARIO_WALKING_MAXSPEED;
						}
						else if (IsRunning) {
							if (vx >= 0.2f)
							{
								vx = MARIO_RUNNING_MAXSPEED;
								state = MARIO_STATE_RUN_MAXSPEED;
							}
							else {
								vx += SPEED_RUNNING_INCREASE;
								state = MARIO_STATE_RUNNING;
							}

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
									//DebugOut(L"line 126 IDLE\n");
									state = MARIO_STATE_IDLE;
								}
							}
						}
					}
				}
				else {////vx<0------->dang di qua trai/////////////
					if (IsWalkingL) {
						if (!IsRunning) {
							vx -= MARIO_ACCELERATE;
							state = MARIO_STATE_WALKING_LEFT;
							if (!IsRunning && vx <= -MARIO_WALKING_MAXSPEED)
								vx = -MARIO_WALKING_MAXSPEED;
						}
						else if (IsRunning) {
							if (vx <= -0.2f)
							{
								vx = -MARIO_RUNNING_MAXSPEED;
								state = MARIO_STATE_RUN_MAXSPEED;
							}
							else {
								vx -= SPEED_RUNNING_INCREASE;
								state = MARIO_STATE_RUNNING;
							}

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
									//DebugOut(L"line 167 IDLE\n");
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
			//DebugOut(L"line 219 Sitting\n");
			state = MARIO_STATE_SIT;
		}
		if (IsDie)
		{
			state = MARIO_STATE_DIE;
		}
		//if (IsHolding)
		//{
		//	//state = MARIO_STATE_HOLDTURTLE;
		//}
		if (!IsHolding && dynamic_cast<CKoopas*>(koo)->IsHeld == true) {
			//koo->IsHeld = false;
			//koo = NULL;
			dynamic_cast<CKoopas*>(koo)->IsHeld = false;
			state = MARIO_STATE_IDLE;
			if (dynamic_cast<CKoopas*>(koo)->GetState() == KOOPAS_STATE_DEFEND)
			{
				dynamic_cast<CKoopas*>(koo)->nx = this->nx;
				dynamic_cast<CKoopas*>(koo)->SubHealth(1);
				//dynamic_cast<CKoopas*>(koo)->SetState(KOOPAS_STATE_ATTACK);
			}
		}
		if (IsHolding&&dynamic_cast<CKoopas*>(koo)->IsHeld)
		{
			if (IsWalking)
			{
				state = MARIO_STATE_HOLDTUTLE_WALK;
			}
			else
			{
				state = MARIO_STATE_HOLDTURTLE;
			}
			if (nx > 0)
				koo->SetPosition(this->x + 15, this->y + 5);
			if (nx < 0)
				koo->SetPosition(this->x - 15, this->y + 5);
		}
		if (IsKicking)
		{
			if (time1 <= 10)
			{
				state = MARIO_STATE_KICKTURTLE;
				time1++;
			}
			else
			{
				IsKicking = false;
				state = MARIO_STATE_IDLE;
			}
		}
		if (IsInPipe)
		{
			vy = ny*0.06f;
		}
	}
	if (IsInMap3 || IsInMap4)
	{
		CalcPotentialCollisions(coObjects, coEvents);
		if (!IsMoving)
		{
			vy = 0;
			vx = 0;
		}
		else { x += dx; y += dy; }
	}
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable++;
		if (GetTickCount64() - untouchable_start > 5000)
		{
			untouchable_start = 0;
			untouchable = 0;
			untouchable = false;
		}
	}

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
		int posObjx = 0;
		int posObjy = 0;


		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.6f;
		y += min_ty * dy + ny * 0.4f;

		if (min_tx != 1 && min_ty != 1 && coEventsResult[1]->obj->GetType() == GType::BRICK) {
			//DebugOut(L"line 247 coevent = brick\n");
			IsOnGround = true;
		}
		/*if (nx != 0) {
			if (!IsWalking)
				state = MARIO_STATE_IDLE;
		}*/
		//if (ny != 0) vy = 0;
		if (ny == -1) if (!IsJumping && !IsFlying) { vy = 0; }
		// Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->nx != 0) {
				//brick or pipe
				if (e->obj->GetType() == GType::BRICK || e->obj->GetType() == GType::PIPE || e->obj->GetType() == GType::QUESTIONBRICK||e->obj->GetType()==GType::UPDOWNWOOD) {
					vx = this->nx*0.04f;
					if (!IsWalking) {
						if (!IsSitting) {
							//DebugOut(L"line 277 IDLE\n");
							state = MARIO_STATE_IDLE;
						}
						else
						{
							state = MARIO_STATE_SIT;
						}
					}
					if (!IsFlying) {
						IsOnGround = true;

					}
				}
				if (e->obj->GetType() == GType::COLORBRICK) { x += dx; }
				if (dynamic_cast<CPortal *>(e->obj))
				{
					CPortal *p = dynamic_cast<CPortal *>(e->obj);
					NextScene = p->GetSceneId();
					NextX = p->GetNextX();
					NextY = p->GetNextY();
					IsPortal = true;
					IsEndGame = false;
					IsInPipe = false;
				}

			}


			if (e->ny != 0) {
				if (e->obj->GetType() == GType::PITEM)
				{
					CPItem *PItem = dynamic_cast<CPItem *>(e->obj);

					if (PItem->GetState() == PITEM_STATE_NORMAL) {

						PItem->SetState(PITEM_STATE_COLLISION);
						PItem->y += NUMBER_10;
					}
				}
				if (e->obj->GetType()==GType::SHINNINGEXTRABRICK)
				{
					ShinningExtraBrick *ExtraBrick = dynamic_cast<ShinningExtraBrick*>(e->obj);
					if (e->ny == 1)
					{
						if (vy < 0) {
							Gravity = 0;
							vy += 0.005;
							if (ExtraBrick->GetState() == EXTRABRICK_STATE_NORMAL)
							{
								ExtraBrick->SetState(EXTRABRICK_STATE_COLLISION);
								if (ExtraBrick->kind == 1)
								{
									this->Coin += 1;
									this->Point += 100;
								}
								else {
									this->Point += 200;
								}
								//ExtraBrick->number -= 1;
							}
							if (IsJumping) {
								IsJumping = false;
								IsFalling = true;
								/*if (!IsSitting)
									DebugOut(L"mario line 323 jump\n");*/
								state = MARIO_STATE_FALLING;
							}
						}

					}
					else {
						z = 0;
						y += e->ny * 0.4f;
						IsOnGround = true;
						if (IsFalling) {
							IsFalling = false;
							if (vx != 0) {
								state = MARIO_STATE_WALKING;
							}
							else
								if (!IsSitting) {
									//DebugOut(L"line 348 IDLE\n");
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
				if (e->obj->GetType() == GType::BROWNLEAF)
				{
					e->obj->SetHealth(0);
					this->Point += 200;
				}
				if (e->obj->GetType() == GType::PIPE)
				{
					CPipe *p = dynamic_cast<CPipe *>(e->obj);
					vy = 0;
					if (p->IsPortal == 1)
					{
						this->CanPiping = true;
					}
					else {
						this->CanPiping = false;
					}
					if (!CanPiping)
					{
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
							y += e->ny * 0.4f;
							IsOnGround = true;
							if (IsFalling) {
								IsFalling = false;
								if (vx != 0) {
									state = MARIO_STATE_WALKING;
								}
								else
									if (!IsSitting) {
										//DebugOut(L"line 348 IDLE\n");
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
					else if (this->CanPiping==true)
					{
						//y += dy;
						//vy = 0;
						if (IsInPipe)
						{
							if (this->ny==1)
							{
								//tren xuong
								this->ny = 1;
								y += dy;
							}
							
						}
						else vy = 0;
					}
					if (e->ny == -1) {
						IsOnGround = true;
						if (IsFalling) {
							IsFalling = false;
							if (vx != 0) {
								state = MARIO_STATE_WALKING;
							}
							else
								if (!IsSitting) {
									//DebugOut(L"line 302 IDLE\n");
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
				if (e->obj->GetType() == GType::UPDOWNWOOD)
				{
					CUpDownWood* wood = dynamic_cast<CUpDownWood*>(e->obj);
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
					else
					{
						wood->SetState(WOOD_STATE_DOWN);
						y += e->ny * 0.4f;
						IsOnGround = true;
						if (IsFalling) {
							IsFalling = false;
							if (vx != 0) {
								state = MARIO_STATE_WALKING;
							}
							else
								if (!IsSitting) {
									//DebugOut(L"line 348 IDLE\n");
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
				if (e->obj->GetType() == GType::SHINNINGBRICK)
				{
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
						z = 0;
						//y += e->ny * 0.4f;
						IsOnGround = true;
						if (IsFalling) {
							IsFalling = false;
							if (vx != 0) {
								state = MARIO_STATE_WALKING;
							}
							else
								if (!IsSitting) {
									//DebugOut(L"line 348 IDLE\n");
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
				if (e->obj->GetType() == GType::COLORBRICK) {
					if (e->ny == 1) {
						y += dy;
						//KO XAY RA CHUYEN GI
					}
					else {
						z = 0;
						IsOnGround = true;
						if (IsFalling) {
							IsFalling = false;
							if (vx != 0) {
								state = MARIO_STATE_WALKING;
							}
							else
								if (!IsSitting) {
									//DebugOut(L"line 302 IDLE\n");
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
				else if (e->obj->GetType() == GType::BRICK ) {
					//DebugOut(L"line 443 MARIO\n");
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
						z = 0;
						y += e->ny * 0.4f;
						IsOnGround = true;
						if (IsFalling) {
							IsFalling = false;
							if (vx != 0) {
								state = MARIO_STATE_WALKING;
							}
							else
								if (!IsSitting) {
									//DebugOut(L"line 348 IDLE\n");
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
				else if (e->obj->GetType() == GType::QUESTIONBRICK) {
					CQuestionBrick *QBrick = dynamic_cast<CQuestionBrick *>(e->obj);
					if (e->ny == 1) {
						if (vy < 0) {
							Gravity = 0;
							vy += 0.005f;

							if (QBrick->GetState() != BRICK_STATE_NOTHINGLEFT) {

								if (QBrick->bricktype == QUESTIONBRICK_TYPE_COIN)
								{
									QBrick->HiddenItem = TYPE_COIN;
								}
								else if (QBrick->bricktype == QUESTIONBRICK_TYPE_MOVING)
								{
									if (this->level == MARIO_LEVEL_SMALL)
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
									//DebugOut(L"line 348 IDLE\n");
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
				else if (e->obj->GetType() == GType::CPBRICK)
				{

					CPBrick *PBrick = dynamic_cast<CPBrick *>(e->obj);
					if (e->ny == 1) {
						if (vy < 0) {
							Gravity = 0;
							vy += 0.005f;

							if (PBrick->GetState() == CPBRICK_STATE_NORMAL) {
								PBrick->SetState(CPBRICK_STATE_NOTHINGLEFT);
							}
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
									//DebugOut(L"line 348 IDLE\n");
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
				else if (e->obj->GetType() == GType::PORTAL)
				{
					CPortal *p = dynamic_cast<CPortal *>(e->obj);
					NextScene = p->GetSceneId();
					NextX = p->GetNextX();
					NextY = p->GetNextY();
					IsPortal = true;
					IsInPipe = false;
					IsEndGame = false;
				}
				else {
					//y += dy;
				}
			}
			else {
				if (e->obj->GetType() == GType::BRICK) {
					IsOnGround = false;
				}

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
				case GType::KOOPAS:
				{
					if (isCollisionWithObj(coObjects->at(i))->nx != 0) {
						if (coObjects->at(i)->GetState() == KOOPAS_STATE_WALKING && !IsAttacking)
						{
							SetState(MARIO_STATE_DIE);
						}
						if (level == 4 && IsAttacking)
						{
							if (coObjects->at(i)->GetState() == KOOPAS_STATE_WALKING)
							{
								coObjects->at(i)->SubHealth(1);
								coObjects->at(i)->SetState(KOOPAS_STATE_UP);
							}
							if (coObjects->at(i)->GetState() == KOOPAS_STATE_DEFEND)
							{
								coObjects->at(i)->SetHealth(1);
								coObjects->at(i)->SetState(KOOPAS_STATE_DIE_UP);
							}
						}
						if (coObjects->at(i)->GetState() == KOOPAS_STATE_DEFEND && IsHolding == false)
						{

							//koo->IsHeld = false;
							IsKicking = true;
							coObjects->at(i)->nx = this->nx;
							coObjects->at(i)->SubHealth(1);
							coObjects->at(i)->SetState(KOOPAS_STATE_ATTACK);
						}
						if (IsHolding && coObjects->at(i)->GetState() == KOOPAS_STATE_DEFEND)
						{
							//state = MARIO_STATE_HOLDTURTLE;
							dynamic_cast<CKoopas*>(coObjects->at(i))->IsHeld = true;
							koo = dynamic_cast<CKoopas*>(coObjects->at(i));
						}


						if (coObjects->at(i)->GetState() == KOOPAS_STATE_UP && IsHolding == false)
						{
							IsKicking = true;
							coObjects->at(i)->nx = this->nx;
							coObjects->at(i)->SubHealth(1);
							coObjects->at(i)->SetState(KOOPAS_STATE_UP_ATTACK);
						}
						if (IsHolding && coObjects->at(i)->GetState() == KOOPAS_STATE_UP)
						{

							//state = MARIO_STATE_HOLDTURTLE;
							dynamic_cast<CKoopas*>(coObjects->at(i))->IsHeld = true;
							koo = dynamic_cast<CKoopas*>(coObjects->at(i));
							//DebugOut(L"STILL HOLD\n");
							//koo->IsHeld = true;
						}
						break;
					}
					if (isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						if (isCollisionWithObj(coObjects->at(i))->ny < 0)
						{
							//DebugOut(L"line 407");
							if (coObjects->at(i)->GetState() == KOOPAS_STATE_WALKING)
							{
								//DebugOut(L"Line 411 set state walking");
								vy -= MARIO_DIE_DEFLECT_SPEED / 2;
								coObjects->at(i)->SubHealth(1);
								coObjects->at(i)->SetState(KOOPAS_STATE_DEFEND);
								break;
							}
							if (coObjects->at(i)->GetState() == KOOPAS_STATE_DEFEND) {
								//DebugOut(L"line 810\n");
								if (this->x >= (coObjects->at(i)->GetX() + (KOOPAS_BBOX_WIDTH / 2)))
								{
									coObjects->at(i)->nx = -1;
								}
								else if (this->x < (coObjects->at(i)->GetX() + (KOOPAS_BBOX_WIDTH / 2)))
								{
									coObjects->at(i)->nx = 1;
								}
								coObjects->at(i)->SetState(KOOPAS_STATE_ATTACK);
								coObjects->at(i)->SubHealth(1);
								break;
							}
							if (coObjects->at(i)->GetState() == KOOPAS_STATE_UP) {
								if (this->x >= coObjects->at(i)->GetX() + (KOOPAS_BBOX_WIDTH / 2))
								{
									coObjects->at(i)->nx = -1;
								}
								else if (this->x < coObjects->at(i)->GetX() + (KOOPAS_BBOX_WIDTH / 2))
								{
									coObjects->at(i)->nx = 1;
								}
								//if(coObjects->at(i)->GetState()==KOOPAS_sT)
								coObjects->at(i)->SetState(KOOPAS_STATE_UP_ATTACK);
								coObjects->at(i)->SubHealth(1);

								//DebugOut(L"Line 404 set state attack");
								vy -= MARIO_JUMP_DEFLECT_SPEED;
								//SweptAABBEx(coObjects->at(i));
								break;
							}
							if (coObjects->at(i)->GetState() == KOOPAS_STATE_ATTACK)
							{
								//DebugOut(L"line 844 attack\n");
								vy -= 0.35f;
								coObjects->at(i)->SetState(KOOPAS_STATE_DEFEND);
								coObjects->at(i)->PlusHealth(1);
								break;
							}
							if (coObjects->at(i)->GetState() == KOOPAS_STATE_UP_ATTACK)
							{
								vy -= MARIO_DIE_DEFLECT_SPEED;
								coObjects->at(i)->SetState(KOOPAS_STATE_UP);
								coObjects->at(i)->PlusHealth(1);
								break;
							}
						}
						else if (isCollisionWithObj(coObjects->at(i))->ny > 0)
						{
							if (this->vy < 0) {
								if (coObjects->at(i)->GetState() == KOOPAS_STATE_WALKING)
								{
									SetState(MARIO_STATE_DIE);
								}
							}
						}
					}
					break;
				}
				case GType::GOOMBA:
				{
					if (isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						if (coObjects->at(i)->GetState() == GOOMBA_STATE_WALKING)
						{
							Point += 200;
							coObjects->at(i)->SubHealth(1);
							vy -= MARIO_DIE_DEFLECT_SPEED;
							//e->obj->IsDie = true;
						}
						//if (isCollisionWithObj(coObjects->at(i))->ny < 0)
						//{
						//	//DebugOut(L"ny:%f\n", isCollisionWithObj(coObjects->at(i))->ny);
						//	
						//}
						//else {
						//	//DebugOut(L"Line 878 mario\n");
						//	SetState(MARIO_STATE_DIE);
						//}
						break;
					}
					if (isCollisionWithObj(coObjects->at(i))->nx != 0 && coObjects->at(i)->GetHealth() == 2)
					{
						//DebugOut(L"health:%f\n", coObjects->at(i)->GetHealth());
						if (level == 4 && IsAttacking)
						{
							coObjects->at(i)->SubHealth(1);
							Point += 200;
						}
						else
						{
							//DebugOut(L"LINE 873 MARIO\n");
							if (coObjects->at(i)->GetHealth() == 2) {
								SetState(MARIO_STATE_DIE);
							}
						}
						break;
					}
					break;
				}
				case GType::ITEM:
				{
					if (coObjects->at(i)->GetState() == GOOMBA_STATE_WALKING)

					{
						if (level == 1) {
							y -= MARIOTAIL_BBOX_TAIL;
							SetLevel(2);

						}
						coObjects->at(i)->SubHealth(1);
					}
					break;

				}
				case GType::FIREGREENPLANT:
				{
					if (isCollisionWithObj(coObjects->at(i))->nx != 0)
					{
						if (level == MARIO_LEVEL_TAIL)
						{
							coObjects->at(i)->SubHealth(1);
							break;
						}
						else {
							SetState(MARIO_STATE_DIE);
						}
						break;
					}
					if (isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						SetState(MARIO_STATE_DIE);
						break;
					}
					break;
				}
				case GType::GREENPLANT:
				{
					if (isCollisionWithObj(coObjects->at(i))->nx != 0)
					{
						SetState(MARIO_STATE_DIE);
					}
					if (isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						SetState(MARIO_STATE_DIE);
					}
					break;
				}
				case GType::FIREPIRAHAPLANT:
				{
					if (isCollisionWithObj(coObjects->at(i))->nx != 0)
					{
						SetState(MARIO_STATE_DIE);
					}
					if (isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						SetState(MARIO_STATE_DIE);
					}
					break;
				}
				case GType::FLYKOOPAS:
				{
					if (isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						if (isCollisionWithObj(coObjects->at(i))->ny < 0)
						{
							//DebugOut(L"ny:%f\n", isCollisionWithObj(coObjects->at(i))->ny);
							if (coObjects->at(i)->GetState() == FLYKOOPAS_STATE_FLY)
							{
								//DebugOut(L"line 893 MARIO\n");
								coObjects->at(i)->SubHealth(1);
								vy -= MARIO_DIE_DEFLECT_SPEED;
								coObjects->at(i)->SetState(FLYKOOPAS_STATE_DISAPPEAR);
								//e->obj->IsDie = true;
							}
						}
						if (isCollisionWithObj(coObjects->at(i))->ny > 0)
						{
							this->SetState(MARIO_STATE_DIE);
						}
					}
					if (isCollisionWithObj(coObjects->at(i))->nx != 0 && isCollisionWithObj(coObjects->at(i))->ny == 0)
					{
						SetState(MARIO_STATE_DIE);
					}
					break;
				}
				case GType::BROWNCOIN:
				{
					//coObjects->at(i) = NULL;
					Coin += 1;
					Point += 50;
					coObjects->at(i)->SubHealth(1);
					break;
				}
				case GType::SHINNINGBRICK:
				{
					if (coObjects->at(i)->GetState() == BRICK_STATE_NORMAL)
					{

						if (isCollisionWithObj(coObjects->at(i))->nx != 0)
						{
							if (level == MARIO_LEVEL_TAIL && IsAttacking)
							{
								if (coObjects->at(i)->GetState() == BRICK_STATE_NORMAL)
								{
									//DebugOut(L"line 926 mario\n");
									coObjects->at(i)->SubHealth(1);
									coObjects->at(i)->SetState(SBRICK_STATE_NOTHINGLEFT);
									break;
									//coObjects->at(i)->SubHealth(1);
								}

							}
							else
							{
								vx = this->nx*0.01f;
								if (!IsWalking) {
									if (!IsSitting) {
										//DebugOut(L"line 277 IDLE\n");
										state = MARIO_STATE_IDLE;
									}
									else
									{
										state = MARIO_STATE_SIT;
									}
								}
								if (!IsFlying) {
									IsOnGround = true;

								}
							}
							break;
						}
						if (isCollisionWithObj(coObjects->at(i))->ny != 0)
						{
							if (isCollisionWithObj(coObjects->at(i))->ny < 0) {
								//y += isCollisionWithObj(coObjects->at(i))->ny * 0.4f;
								IsOnGround = true;
								if (IsFalling) {
									IsFalling = false;
									if (vx != 0) {
										state = MARIO_STATE_WALKING;
									}
									else
										if (!IsSitting) {
											//DebugOut(L"line 348 IDLE\n");
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
							else {
								coObjects->at(i)->SubHealth(1);
								coObjects->at(i)->SetState(SBRICK_STATE_NOTHINGLEFT);
								break;
							}
							break;
						}
						break;
					}
					if (coObjects->at(i)->GetState() == SBRICK_STATE_TURN_COIN)
					{
						coObjects->at(i)->SetHealth(0);
						this->Coin += 1;
						this->Point += 100;
						//x += dx;
						//y += dy;
					}
					break;
				}
				case GType::BROWNLEAF:
				{
					if (isCollisionWithObj(coObjects->at(i))->nx != 0 || isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						CLeaf *item = dynamic_cast<CLeaf *>(coObjects->at(i));
						if (item->GetHealth() == 1)
						{
							if (this->level == MARIO_LEVEL_SMALL) {
								y -= MARIOTAIL_BBOX_TAIL;
								this->SetLevel(2);
							}
							if (this->level != MARIO_LEVEL_SMALL)
							{
								y -= MARIOTAIL_BBOX_TAIL;
								this->SetLevel(this->level + 1);
							}
							item->SubHealth(1);
						}

						//coObjects->at(i)->SubHealth(1);
					}
					break;
				}
				case GType::REDGOOMBA:
				{
					if (isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						if (isCollisionWithObj(coObjects->at(i))->ny < 0)
						{
							//DebugOut(L"ny:%f\n", isCollisionWithObj(coObjects->at(i))->ny);
							if (coObjects->at(i)->GetHealth() == 3)
							{
								coObjects->at(i)->SubHealth(1);
								vy -= MARIO_DIE_DEFLECT_SPEED;
								//e->obj->IsDie = true;
							}
							else if (coObjects->at(i)->GetHealth() == 2)
							{
								coObjects->at(i)->SubHealth(1);
								coObjects->at(i)->SetState(REDGOOMBA_STATE_DIE);
								vy -= MARIO_DIE_DEFLECT_SPEED;
								//e->obj->IsDie = true;
							}
							break;
						}

						else {
							if (coObjects->at(i)->GetState() != REDGOOMBA_STATE_DIE_UP)
								SetState(MARIO_STATE_DIE);
						}
						break;
					}
					if (isCollisionWithObj(coObjects->at(i))->nx != 0 && coObjects->at(i)->GetHealth() > 1)
					{
						//DebugOut(L"health:%f\n", coObjects->at(i)->GetHealth());
						if (level == 4 && IsAttacking)
						{
							/*if (this->nx > 0)
								x -= 10;*/
							coObjects->at(i)->SetState(REDGOOMBA_STATE_DIE_UP);
							coObjects->at(i)->SetHealth(1);
							this->vy = -0.01f;
						}
						else
						{

							if (coObjects->at(i)->GetHealth() > 1) {
								SetState(MARIO_STATE_DIE);
							}
						}
						break;
					}
					break;
				}
				case GType::ENDGAMEITEM:
				{
					if (coObjects->at(i)->GetState() == STATE_STAR)
					{
						IsEndGame = true;
						coObjects->at(i)->SetState(STATE_STAR_UP);
						break;
					}
					if (coObjects->at(i)->GetState() == STATE_MUSHROOM)
					{
						IsEndGame = true;
						coObjects->at(i)->SetState(STATE_MUSHROOM_UP);
						break;
					}
					if (coObjects->at(i)->GetState() == STATE_PLANT)
					{
						IsEndGame = true;
						coObjects->at(i)->SetState(STATE_PLANT_UP);
						break;
					}

					break;
				}
				case GType::NODE:
				{
					CNode *p = dynamic_cast<CNode *>(coObjects->at(i));
					if (this->vx > 0)
					{
						if (this->x > p->x)
						{
							this->IsMoving = false;
						}
						else
						{
							x += dx;
						}
					}
					if (this->vx < 0)
					{
						if (this->x <= p->x)
						{
							this->IsMoving = false;
						}
						else
						{
							x += dx;
						}
					}
					if (this->vy > 0)
					{
						if (this->y > p->y)
						{
							this->IsMoving = false;
						}
						else
						{
							y += dy;
						}
					}
					if (this->vy < 0)
					{
						if (this->y <= p->y)
						{
							this->IsMoving = false;
						}
						else
						{
							y += dy;
						}
					}

					//this->IsMoving = false;
					this->NextScene = p->GetSceneIDOfNode();
					up = p->IsUp();
					down = p->IsDown();
					right = p->IsRight();
					left = p->IsLeft();
					break;
				}
				case GType::FLYUPDOWNKOOPAS:
				{
					if (isCollisionWithObj(coObjects->at(i))->ny != 0)
					{
						if (isCollisionWithObj(coObjects->at(i))->ny < 0)
						{
							//DebugOut(L"ny:%f\n", isCollisionWithObj(coObjects->at(i))->ny);
							if (coObjects->at(i)->GetState() == FLY_STATE)
							{
								vy -= 0.7f;
								coObjects->at(i)->SetState(DISAPPEAR_STATE);
								//e->obj->IsDie = true;
							}
						}
						if (isCollisionWithObj(coObjects->at(i))->ny > 0)
						{
							this->SetState(MARIO_STATE_DIE);
						}
					}
					if (isCollisionWithObj(coObjects->at(i))->nx != 0 && isCollisionWithObj(coObjects->at(i))->ny == 0)
					{
						SetState(MARIO_STATE_DIE);
					}
					break;
				}
				case GType::QUESTIONBRICK:
				{
					if (isCollisionWithObj(coObjects->at(i))->nx != 0)
					{
						if(this->level==4&&IsAttacking)
						{CQuestionBrick *QBrick = dynamic_cast<CQuestionBrick *>(coObjects->at(i));
						if (QBrick->GetState() != BRICK_STATE_NOTHINGLEFT) {

							if (QBrick->bricktype == QUESTIONBRICK_TYPE_COIN)
							{
								QBrick->HiddenItem = TYPE_COIN;
							}
							else if (QBrick->bricktype == QUESTIONBRICK_TYPE_MOVING)
							{
								if (this->level == MARIO_LEVEL_SMALL)
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
					}
					}
					break;
				}
				case GType::BOOMERANG:
				{
					this->SetState(MARIO_STATE_DIE);
					break;
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
		//DebugOut(L"x y:%f %f\n",NextX,NextY);
	if (vy < 0)
	{
		ny = -1;
	}
	else if (vy > 0)
	{
		ny = 1;
	}
	//DebugOut(L"ny:%d\n", ny);
}

void CMario::Render()
{
	if (!IsInMap3 && !IsInMap4) {
		if (state == MARIO_STATE_DIE) {
			ani = MARIO_ANI_DIE;
		}
		else
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else
					ani = MARIO_ANI_BIG_IDLE_LEFT;
				if (state == MARIO_STATE_HOLDTURTLE)
				{
					if (nx > 0)
						ani = MARIO_ANI_BIG_HOLDTURTLE_IDLE_RIGHT;
					if (nx < 0)
						ani = MARIO_ANI_BIG_HOLDTURTLE_IDLE_LEFT;
				}
				if (state == MARIO_STATE_HOLDTUTLE_WALK)
				{
					if (nx > 0)
						ani = MARIO_ANI_BIG_HOLDTURTLE_WALK_RIGHT;
					if (nx < 0)
						ani = MARIO_ANI_BIG_HOLDTURTLE_WALK_LEFT;
				}
				if (state == MARIO_STATE_KICKTURTLE)
				{
					//DebugOut(L"ANI line 670\n");
					if (nx > 0)
						ani = MARIO_ANI_BIG_KICK_RIGHT;
					if (nx < 0)
					{
						ani = MARIO_ANI_BIG_KICK_LEFT;
					}
				}
				if (state == MARIO_STATE_WALKING_RIGHT|| IsEndGame==true)
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
					//DebugOut(L"FALLFLY ANI");
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
				if (IsInPipe)
				{
					ani = MARIO_BIG_ANI_GO_IN_PIPE;
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

				if (state == MARIO_STATE_WALKING_RIGHT||IsEndGame==true)
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
				if (IsInPipe)
				{
					ani = MARIOTAIL_ANI_GO_IN_PIPE;
				}
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0)
					ani = MARIOWHITE_ANI_IDLE_RIGHT;
				else
					ani = MARIOWHITE_ANI_IDLE_LEFT;
				if (state == MARIO_STATE_WALKING_RIGHT||IsEndGame==true)
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
				if (IsInPipe)
				{
					ani = MARIO_FIRE_ANI_GO_IN_PIPE;
				}

			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else
					ani = MARIO_ANI_SMALL_IDLE_LEFT;
				if (state == MARIO_STATE_WALKING_RIGHT||IsEndGame==true)
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
				if (IsInPipe)
				{
					ani = MARIO_SMALL_ANI_GO_IN_PIPE;
				}

			}
		}
	}
	if (IsInMap3)
	{
		if (level == MARIO_LEVEL_SMALL)
		{

			ani = MARIO_START_SCENE_ANI;
		}
		if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
		{
			ani = MARIO_ANI_WORLDMAP_LEVELBIG;
		}
		if (level == MARIO_LEVEL_TAIL)
		{
			ani = MARIO_ANI_WORLDMAP_LEVELTAIL;
		}
	}
	if (IsInMap4)
	{
		ani = MARIO_ANI_START_SCENE;
	}
	int alpha = 255;
	/*if (untouchable==0)
	{
		if(untouchable%3==0)
			animation_set->at(ani)->Render(x, y, 32);
		else 
			animation_set->at(ani)->Render(x, y, 255);
		return;

	}
	else { animation_set->at(ani)->Render(x, y, 128); 
		return;
	}*/

		//alpha = 128;
	//if (nx == 1)
	for (int i = 0; i < LstWeapon.size(); i++) {
		LstWeapon[i]->Render();
	}
	animation_set->at(ani)->Render(x, y, 255);
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

		this->state = MARIO_STATE_RUNNING;
		break;
	case MARIO_STATE_ATTACK_TAIL:
		this->state = MARIO_STATE_ATTACK_TAIL;
		IsAttacking = true;
		break;
	case MARIO_STATE_FIRE:
		if (nx == 1)
		{
			if (LstWeapon[0]->GetHealth() == 0)
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
		if (!IsOnGround && !IsJumping && !IsFlying&&IsFalling)
			return;
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
							Gravity = -GRAVITY_FALLSLOW1;
							IsFallSlow = true;
						}
					}
				}
				if (IsFlying)
				{
					//DebugOut(L"fly\n");
					if (time <= 4000)
					{

						//IsFlying = true;
						IsFalling = false;
						Gravity = -GRAVITY_DECREASE_UPFLYING;
						vx -= nx * 0.02f;
						IsFlyup = true;
						//this->state = MARIO_STATE_FLY;
					}
					else
					{
						//IsFlying = false;
						Gravity = -GRAVITY_FALLSLOW;
						IsFlyup = false;
						IsFallSlow = true;
						//DebugOut(L"Fly fall slow");
						if (nx > 0) {

							vx -= 0.02f;
							if (vx <= 0.03f)
								vx = 0.03f;
						}
						else if (nx < 0)
						{
							vx += 0.02f;
							if (vx <= -0.03f)
								vx = -0.03f;
						}
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
			if (level != 1)
			{
				StartUntouchable();
				level -= 1;
			}
			else
			{

				vy = -MARIO_DIE_DEFLECT_SPEED / 2;
			IsDie = true;
			}
		}
		else
		{
			//IsDie = false;
			//this->state = MARIO_STATE_IDLE;
		}
		break;
	case MARIO_STATE_HOLDTURTLE:
		IsHolding = true;
		break;
	case MARIO_STATE_KICKTURTLE:
		//IsKicking = true;
		this->state = MARIO_STATE_KICKTURTLE;
		//DebugOut(L"KICK\n");
		break;
	case MARIO_STATE_MOVE_UP:
	{
		IsMoving = true;
		vy = -MARIO_WALKING_SPEED;
		this->state = MARIO_STATE_MOVE_UP;
		vx = 0;
		break;
	}
	case MARIO_STATE_MOVE_DOWN:
	{
		IsMoving = true;
		vy = MARIO_WALKING_SPEED;
		vx = 0;
		break;
	}
	case MARIO_STATE_MOVE_RIGHT:
	{
		IsMoving = true;
		vx = MARIO_WALKING_SPEED;
		vy = 0;
		break;
	}
	case MARIO_STATE_MOVE_LEFT:
	{
		IsMoving = true;
		vx = -MARIO_WALKING_SPEED;
		vy = 0;
		break;
	}
	case MARIO_STATE_IN_PIPE:
	{
		//vy = MARIO_WALKING_SPEED;
		IsInPipe = true;
		this->state = MARIO_STATE_IN_PIPE;
		break;
	}
	case MARIO_STATE_OUT_PIPE:
	{
		//vy = -MARIO_WALKING_SPEED;
		OutPipeY = this->y;
		IsOuttingPipe = true;
		this->state = MARIO_STATE_OUT_PIPE;

		break;
	}
	default:
		break;
	}

}
CMario *CMario::__instance = NULL;
CMario * CMario::GetInstance()
{
	if (__instance == NULL) __instance = new CMario(0, 0);
	return __instance;
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	//top = y;
	if (!IsInMap3 && !IsInMap4)
	{
		if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
		{
			top = y;
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
			if (state == MARIO_STATE_SIT)
			{
				right = x + 10;
				bottom = y + MARIO_SMALL_BBOX_HEIGHT + 2;
			}

		}
		if (level == MARIO_LEVEL_SMALL)
		{
			top = y;
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		}
		if (level == MARIO_LEVEL_TAIL && IsAttacking)
		{
			if (nx == 1)
			{
				left = x + 10;
				right = left + MARIO_TAIL_BBOX_WIDTH_ATTACK;
			}
			else if(nx==-1)
			{
				left = x - 10;
				right = left + MARIO_TAIL_BBOX_WIDTH_ATTACK;
			}
			//left = x + 15;
			top = y+16;
			bottom = top + 12;
		}
		else if (level == 4 && !IsAttacking)
		{
			if (nx == 1)
			{
				left = x + 10;
				top = y;
				right = left + MARIO_TAIL_BBOX_WIDTH;
				bottom = y + MARIO_BIG_BBOX_HEIGHT;
			}
			if (nx == -1)
			{
				left = x;
				top = y;
				right = left + MARIO_TAIL_BBOX_WIDTH;
				bottom = y + MARIO_BIG_BBOX_HEIGHT;
			}
			if (state == MARIO_STATE_SIT)
			{
				right = x + 10;
				bottom = y + MARIO_SMALL_BBOX_HEIGHT + 2;
			}
		}
	}
	if (IsInMap3)
	{
		top = y;
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	if (IsInMap4)
	{
		top = y;
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_START_SCENE_BBOX_HEIGHT;
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
	SetPosition(x, start_y);
	SetSpeed(0, 0);
}

#pragma once
#include "GameObject.h"
#include"Utils.h"
#define REDGOOMBA_JUMP_HEIGHT	25
#define REDGOOMBA_SMALL_BBOX_HEIGTH	17
#define REDGOOMBA_SMALL_BBOX_WIDTH	16
#define REDGOOMBA_BIG_BBOX_HEIGHT	24
#define REDGOOMBA_BIG_BBOX_WIDTH	21
#define REDGOOMBA_GRAVITY	0.001f
#define REDGOOMBA_VX	0.03f
#define REDGOOMBA_DEFLECT_SPEED_LOW	0.2f
#define REDGOOMBA_DEFLECT_SPEED_HIGH	0.35f

#define REDGOOMBA_STATE_ONGROUND	100
#define REDGOOMBA_STATE_READY	200
#define REDGOOMBA_STATE_JUMP	300
#define REDGOOMBA_STATE_NOWINGS	400
#define REDGOOMBA_STATE_DIE		500
#define REDGOOMBA_STATE_DIE_UP	600

#define REDGOOMBA_ANI_JUMP	0
#define REDGOOMBA_ANI_MOVING	1
#define REDGOOMBA_ANI_NOWINGS	2
#define REDGOOMBA_ANI_DIE_UP	3
#define REDGOOMBA_ANI_DIE		4

class RedGoomba: public CGameObject
	
{
public:
	DWORD timeonground;
	bool IsOnGround,IsReady,IsJump,IsNoWings,Isdie,Isdieup;
	int NoJump,NoJumpHigh;
	int ani=0;
	float startY;
	int time;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	RedGoomba();

};


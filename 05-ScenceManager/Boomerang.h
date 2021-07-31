#pragma once
#include"GameObject.h"
#include "Mario.h"
#define BOOMERANG_ANI_ONHAND	0
#define BOOMERANG_ANI_FLY		1
#define	BOOMERANG_STATE_ATTACK	100
#define BOOMERANG_STATE_DOWN_ATTACK	200
#define BOOMERANG_STATE_MOVING_BACK	300
#define BOMERANG_BBOX_WIDTH 16
#define BOMERANG_BBOX_HEIGHT 16
#define GRAVITY		0.00006f
#define BOOMERANG_ATTACK_SPEEDX		0.1f
#define BOOMERANG_ATTACK_SPEEDY		0.05f
#define BOOMERANG_DOWN_SPEEDX	0.08f
#define	BOOMERANG_DOWN_SPEEDY	0.1f
#define BOOMERANG_BACK_SPEEDX	0.1f
class CBoomerang:public CGameObject
{
	float maxX, maxY;
public:
	CBoomerang(){
		nx = 1;
		type = GType::BOOMERANG;
		//SetHealth(1);
		health = 0;
		SetState(BOOMERANG_STATE_ATTACK);
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(21);
		SetAnimationSet(ani_set);
		//IsMovingObj = false;
	};
	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
typedef CBoomerang	*LPBOOMERANG;


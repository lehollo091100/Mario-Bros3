#pragma once
#include"GameObject.h"
#define BOOMERANG_ANI_ONHAND	0
#define BOOMERANG_ANI_FLY		1
#define	BOOMERANG_STATE_ATTACK	100
#define BOOMERANG_STATE_DOWN_ATTACK	200
#define BOOMERANG_STATE_MOVING_BACK	300
#define BOMERANG_BBOX_WIDTH 16
#define BOMERANG_BBOX_HEIGHT 16
#define GRAVITY		0.00006f
class CBoomerang:public CGameObject
{
	float maxX, maxY;
public:
	CBoomerang(){
		type = GType::BOOMERANG;
		//SetHealth(1);
		health = 0;
		SetState(BOOMERANG_STATE_ATTACK);
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(21);
		SetAnimationSet(ani_set);
	};
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
typedef CBoomerang	*LPBOOMERANG;


#pragma once
#include "GameObject.h"
#include"Utils.h"
#include "points.h"
#define COIN_WIDTH 14
#define COIN_HEIGHT	16
#define COIN_ANI_APPEAR	0
#define POINT_100_ANI	1
#define COIN_STATE_UP	100
#define COIN_STATE_FALLING	200
#define COIN_STATE_COLLISION	300
//#define LEAF_STATE_UP_FALLING	300
#define RANGE	15
#define MAXHEIGHT	15
#define SPEEDUP	0.3f
#define SPEEDDOWN	0.07f
#define POINT_UP_SPEED	0.05f

class CCoinInBrick : public CGameObject
{
public:
	vector<LPGAMEOBJECT> LstPoint;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int MaxHeight;
	float rangeX, startX, startY;
	bool IsUp, IsFalling, IsPoint;
	int time;


public:

	CCoinInBrick() {
		type = GType::COININBRICK;
		SetHealth(2);
		time = 0;
		ny = -1;
		SetState(COIN_STATE_UP);
		vy = ny * SPEEDUP;
		IsUp = true;
		IsFalling =IsPoint = false;
		startX = startY = 0;
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(12);
		SetAnimationSet(ani_set);
	};
	virtual void SetState(int state);
};
typedef CCoinInBrick *LPCOININBRICK;




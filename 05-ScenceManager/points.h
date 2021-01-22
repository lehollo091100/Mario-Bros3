#pragma once
#include "GameObject.h"
#include"Utils.h"

#define POINT_ANI_APPEAR	0
#define POINT_STATE_UP	100
//#define LEAF_STATE_UP_FALLING	300
#define RANGE	15
#define MAXHEIGHT	20
#define SPEEDUP	0.02f
#define SPEEDDOWN	0.09f
class Cpoints: public CGameObject
{
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int MaxHeight;
	float startY;
	bool IsUp;


public:

	Cpoints() {
		type = GType::EFFECT;
		SetHealth(1);
		ny = -1;
		SetState(POINT_STATE_UP);
		vy = ny * SPEEDUP;
		IsUp = true;
		startY = 0;
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(52);
		SetAnimationSet(ani_set);
	};
	virtual void SetState(int state);
};


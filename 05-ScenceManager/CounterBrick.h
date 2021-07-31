#pragma once
#include "GameObject.h"
#define COUNTERBRICK_STATE_NORMAL	0
#define COUNTERBRICK_STATE_CHANGE	100
#define COUNTERBRICK_ANI_BLANK	0
#define COUNTERBRICK_ANI_NORMAL		1
#define COUNTERBRICK_WIDTH	16
#define COUNTERBRICK_SPEED	0.1f
#define RANGE	15
#define COUNTERBRICK_DEFLECT_SPEED	0.15f

 class CounterBrick :public CGameObject
{	
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	void SetState(int state);
public:
	float startX;
	float width;
	CounterBrick(int t);
};


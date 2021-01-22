#pragma once
#include "GameObject.h"
#include"Utils.h"
#include "Game.h"
#define LEAF_BBOX_WIDTH	16
#define LEAF_STATE_UP	100
#define LEAF_STATE_FALLING	200
#define LEAF_STATE_UP_FALLING	300
#define RANGE	25
#define LEAF_ANI_RIGHT	0
#define LEAF_ANI_LEFT	1
#define MAXHEIGHT	30
#define SPEEDUP	0.8f
#define SPEEDDOWN	0.03f
#define SPEEDX 0.02f
class CLeaf: public CGameObject
{
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int MaxHeight;
	float rangeX, startX,startY;
	bool IsUp, IsFalling,IsFallUp;


public:
	
	CLeaf() {
		type = GType::BROWNLEAF;
		SetHealth(1);
		nx = 1;
		ny = -1;
		SetState(LEAF_STATE_UP);
		vy = ny * SPEEDUP;
		IsUp = true;
		IsFalling = IsFallUp = false;
		startX =startY= 0;
		
	};
	virtual void SetState(int state);
};
typedef CLeaf *LPLEAF;
